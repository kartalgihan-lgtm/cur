const ROLE_CLAIM =
  'http://schemas.microsoft.com/ws/2008/06/identity/claims/role';

type JwtFieldGetter = (field: string) => unknown;

export class AuthRoleHelper {
  private cachedClaim: unknown = undefined;
  private cachedRoles: string[] = [];
  private cachedRoleSet: Set<string> = new Set<string>();

  constructor(private readonly getFieldFromJWT: JwtFieldGetter) {}

  get role(): string {
    return this.roles[0] ?? '';
  }

  get isAdmin(): boolean {
    return this.hasRole('Admin');
  }

  get isUser(): boolean {
    return this.hasRole('User');
  }

  get isNetworkUser(): boolean {
    return this.hasRole('Network');
  }

  private get roles(): string[] {
    const claim = this.readRoleClaim();
    if (this.cachedClaim === claim) {
      return this.cachedRoles;
    }

    const roles = normalizeRoles(claim);
    this.cachedClaim = claim;
    this.cachedRoles = roles;
    this.cachedRoleSet = new Set(roles);
    return roles;
  }

  private get roleSet(): ReadonlySet<string> {
    // Ensure cache is refreshed before membership checks.
    this.roles;
    return this.cachedRoleSet;
  }

  private hasRole(role: string): boolean {
    return this.roleSet.has(role);
  }

  private readRoleClaim(): unknown {
    try {
      return this.getFieldFromJWT(ROLE_CLAIM);
    } catch {
      return '';
    }
  }
}

function normalizeRoles(value: unknown): string[] {
  if (!value) {
    return [];
  }

  if (Array.isArray(value)) {
    return value
      .filter((role): role is string => typeof role === 'string')
      .map((role) => role.trim())
      .filter((role) => role.length > 0);
  }

  if (typeof value === 'string') {
    const trimmed = value.trim();
    if (!trimmed) {
      return [];
    }

    if (trimmed.startsWith('[') && trimmed.endsWith(']')) {
      try {
        const parsed = JSON.parse(trimmed);
        if (Array.isArray(parsed)) {
          return parsed
            .filter((role): role is string => typeof role === 'string')
            .map((role) => role.trim())
            .filter((role) => role.length > 0);
        }
      } catch {
        // Fall through to comma-splitting if JSON parsing fails.
      }
    }

    return trimmed
      .split(',')
      .map((role) => role.trim())
      .filter((role) => role.length > 0);
  }

  return [];
}
