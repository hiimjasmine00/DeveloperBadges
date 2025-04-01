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
    static inline constexpr std::array badgeNames = {
        "No Badge",
        "Mod Developer",
        "Verified Developer",
        "Index Staff",
        "Lead Developer"
    };
    static inline constexpr std::array badgeDescriptions = {
        "does not have a badge",
        "is a <ca>mod developer</c> for <cy>Geode</c>.\n"
        "They have created mods that are available on the <cy>Geode mod index</c>.\n"
        "They will have to have new mods and mod updates approved by the <cd>index staff</c>",
        "is a <cp>verified mod developer</c> for <cy>Geode</c>.\n"
        "They can update their mods on the <cy>Geode mod index</c> without the need for approval.\n"
        "They will still have to have new mods approved by the <cd>index staff</c>",
        "is a member of the <cd>index staff</c> for <cy>Geode</c>.\n"
        "They can approve or reject mods uploaded to the <cy>Geode mod index</c>",
        "is a <co>lead developer</c> of <cy>Geode</c>.\n"
        "They are part of the main development team and have significant contributions to the <cy>Geode ecosystem</c>"
    };
    static inline std::vector<DeveloperBadge> developerBadges;

    static DeveloperBadge& badgeForUser(int);
    static cocos2d::ccColor3B getCommentColor(BadgeType);
    static void showBadgeInfo(std::string, BadgeType);
};
