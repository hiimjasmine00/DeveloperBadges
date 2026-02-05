#include <ccTypes.h>

struct DeveloperBadge {
    int id;
    int type;
    std::string name;
};

namespace DeveloperBadges {
    extern std::map<int, DeveloperBadge> developerBadges;

    DeveloperBadge* badgeForUser(int);
    void showBadgeInfo(std::string_view, int);
}
