using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Mapster;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;

public class AdminUsersController : ControllerBase
{
    private readonly UserManager<ApplicationUser> userManager;

    public AdminUsersController(UserManager<ApplicationUser> userManager)
    {
        this.userManager = userManager;
    }

    [HttpGet("users")]
    [Authorize(Roles = UsersSeeding.AdminRole)]
    public async Task<List<AdminAppUsersResponseDto>> GetUsers()
    {
        var users = await userManager.GetUsersInRoleAsync(UsersSeeding.UserRole);

        var usersResponse = users
             .OrderByDescending(x => x.LastHeartbeat)
             .ToList()
             .Adapt<List<AdminAppUsersResponseDto>>();

        var networkUserIds = (await userManager.GetUsersInRoleAsync(UsersSeeding.NetworkRole))
            .Select(networkUser => networkUser.Id)
            .ToHashSet();
        foreach (var user in usersResponse)
        {
            user.IsNetworkUser = networkUserIds.Contains(user.Id);
        }

        return usersResponse;
    }
}
