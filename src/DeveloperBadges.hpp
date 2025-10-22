#include <ccTypes.h>

struct DeveloperBadge {
    int id;
    int type;
    std::string name;
};

class DeveloperBadges {
public:
    static std::map<int, DeveloperBadge> developerBadges;

    static DeveloperBadge* badgeForUser(int);
    static void showBadgeInfo(const std::string&, int);
};
