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

        var networkUsers = await userManager.GetUsersInRoleAsync(UsersSeeding.NetworkRole);
        foreach (var user in usersResponse)
        {
            user.IsNetworkUser = networkUsers.FirstOrDefault(x => x.Id.Equals(user.Id)) != null;
        }

        return usersResponse;
    }
}
