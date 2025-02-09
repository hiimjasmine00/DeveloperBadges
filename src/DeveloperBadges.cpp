#include "DeveloperBadges.hpp"
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/ranges.hpp>

using namespace geode::prelude;

#define BADGE_URL "https://raw.githubusercontent.com/hiimjasmine00/developer-badges/master/badges/{}.json"

DeveloperBadge DeveloperBadges::badgeForUser(int id) {
    auto badge = ranges::find(DEVELOPER_BADGES, [id](const DeveloperBadge& badge) {
        return badge.id == id && badge.badge != BadgeType::None;
    });
    return badge.has_value() ? badge.value() : DeveloperBadge { 0, "", BadgeType::None };
}

ccColor3B DeveloperBadges::getCommentColor(BadgeType badge) {
    switch (badge) {
        case BadgeType::ModDeveloper: return Mod::get()->getSettingValue<ccColor3B>("mod-developer-color");
        case BadgeType::VerifiedDeveloper: return Mod::get()->getSettingValue<ccColor3B>("verified-developer-color");
        case BadgeType::IndexStaff: return Mod::get()->getSettingValue<ccColor3B>("index-staff-color");
        case BadgeType::LeadDeveloper: return Mod::get()->getSettingValue<ccColor3B>("lead-developer-color");
        default: return ccColor3B { 255, 255, 255 };
    }
}

void DeveloperBadges::showBadgeInfo(std::string username, BadgeType type) {
    auto badgeName = "";
    auto badgeDesc = "";
    switch (type) {
        case BadgeType::ModDeveloper:
            badgeName = "Mod Developer";
            badgeDesc = "is a <ca>mod developer</c> for <cy>Geode</c>.\n"
            "They have created mods that are available on the <cy>Geode mod index</c>.\n"
            "They will have to have new mods and mod updates approved by the <cd>index staff</c>";
            break;
        case BadgeType::VerifiedDeveloper:
            badgeName = "Verified Developer";
            badgeDesc = "is a <cp>verified mod developer</c> for <cy>Geode</c>.\n"
            "They can update their mods on the <cy>Geode mod index</c> without the need for approval.\n"
            "They will still have to have new mods approved by the <cd>index staff</c>";
            break;
        case BadgeType::IndexStaff:
            badgeName = "Index Staff";
            badgeDesc = "is a member of the <cd>index staff</c> for <cy>Geode</c>.\n"
            "They can approve or reject mods uploaded to the <cy>Geode mod index</c>";
            break;
        case BadgeType::LeadDeveloper:
            badgeName = "Lead Developer";
            badgeDesc = "is a <co>lead developer</c> of <cy>Geode</c>.\n"
            "They are part of the main development team and have significant contributions to the <cy>Geode ecosystem</c>";
            break;
        default:
            badgeName = "";
            badgeDesc = "";
            break;
    }

    FLAlertLayer::create(badgeName, fmt::format("<cg>{}</c> {}.", username, badgeDesc), "OK")->show();
}
