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
    std::string name;
    BadgeType badge;
};

class DeveloperBadges {
public:
    static inline std::vector<DeveloperBadge> DEVELOPER_BADGES;
    static inline std::set<int> LOADED_IDS;

    static DeveloperBadge badgeForUser(int);
    static cocos2d::ccColor3B getCommentColor(BadgeType);
    static void showBadgeInfo(std::string, BadgeType);
};
