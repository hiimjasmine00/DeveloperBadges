#include <ccTypes.h>

enum class BadgeType {
    None,
    ModDeveloper,
    VerifiedDeveloper,
    IndexStaff,
    LeadDeveloper
};

struct DeveloperBadge {
    int id;
    BadgeType badge;
    std::string name;
};

class DeveloperBadges {
public:
    static inline std::vector<DeveloperBadge> developerBadges;

    static DeveloperBadge& badgeForUser(int);
    static cocos2d::ccColor3B getCommentColor(BadgeType);
    static void showBadgeInfo(const std::string&, BadgeType);
};
