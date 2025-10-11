#include <ccTypes.h>
struct DeveloperBadge {
    int id;
    int type;
    std::string name;
};

class DeveloperBadges {
public:
    static inline std::vector<DeveloperBadge> developerBadges;

    static DeveloperBadge* badgeForUser(int);
    static void showBadgeInfo(const std::string&, int);
};
