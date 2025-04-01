#include "DeveloperBadges.hpp"
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

DeveloperBadge& DeveloperBadges::badgeForUser(int id) {
    static DeveloperBadge empty = { 0, "", BadgeType::None };
    auto badge = std::ranges::find_if(developerBadges, [id](const DeveloperBadge& badge) {
        return badge.id == id && badge.badge != BadgeType::None;
    });
    return badge != developerBadges.end() ? *badge : empty;
}

ccColor3B DeveloperBadges::getCommentColor(BadgeType badge) {
    auto mod = Mod::get();
    switch (badge) {
        case BadgeType::ModDeveloper: return mod->getSettingValue<ccColor3B>("mod-developer-color");
        case BadgeType::VerifiedDeveloper: return mod->getSettingValue<ccColor3B>("verified-developer-color");
        case BadgeType::IndexStaff: return mod->getSettingValue<ccColor3B>("index-staff-color");
        case BadgeType::LeadDeveloper: return mod->getSettingValue<ccColor3B>("lead-developer-color");
        default: return ccColor3B { 255, 255, 255 };
    }
}

void DeveloperBadges::showBadgeInfo(std::string username, BadgeType type) {
    auto badgeType = (int)type;
    FLAlertLayer::create(
        badgeType < badgeNames.size() ? badgeNames[badgeType] : badgeNames[0],
        fmt::format("<cg>{}</c> {}.", username, badgeType < badgeDescriptions.size() ? badgeDescriptions[badgeType] : badgeDescriptions[0]),
        "OK"
    )->show();
}
