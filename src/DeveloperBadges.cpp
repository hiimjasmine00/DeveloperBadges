#include "DeveloperBadges.hpp"
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/loader/GameEvent.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

#define BADGES_URL "https://badges.hiimjasmine00.com/developer"

$execute {
    new EventListener(+[](GameEvent*) {
        web::WebRequest().get(BADGES_URL).listen([](web::WebResponse* res) {
            if (!res->ok() || !res->json().isOk()) return *res;

            auto badges = res->json().unwrapOr(matjson::Value::array());
            if (!badges.isArray()) return *res;

            DeveloperBadges::developerBadges = ranges::reduce<std::vector<DeveloperBadge>>(
                badges.asArray().unwrap(),
                [](std::vector<DeveloperBadge>& vec, const matjson::Value& value) {
                    if (!value.contains("id") || !value["id"].isNumber() || !value.contains("name") ||
                        !value["name"].isString() | !value.contains("badge") || !value["badge"].isNumber()) return;

                    vec.push_back({
                        .id = value["id"].as<int>().unwrap(),
                        .badge = (BadgeType)value["badge"].as<int>().unwrap(),
                        .name = value["name"].asString().unwrap()
                    });
                }
            );

            return *res;
        });
    }, GameEventFilter(GameEventType::Loaded));
}

DeveloperBadge& DeveloperBadges::badgeForUser(int id) {
    static DeveloperBadge empty = { 0, BadgeType::None, "" };
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

constexpr std::array names = {
    "No Badge",
    "Mod Developer",
    "Verified Developer",
    "Index Staff",
    "Lead Developer"
};

constexpr std::array descriptions = {
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

void DeveloperBadges::showBadgeInfo(const std::string& username, BadgeType type) {
    auto badgeType = (int)type;
    FLAlertLayer::create(
        badgeType < names.size() ? names[badgeType] : names[0],
        fmt::format("<cg>{}</c> {}.", username, badgeType < descriptions.size() ? descriptions[badgeType] : descriptions[0]),
        "OK"
    )->show();
}
