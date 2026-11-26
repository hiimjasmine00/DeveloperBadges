#include <Geode/loader/Mod.hpp>
#include <Geode/loader/ModEvent.hpp>
#include <dasshu.badgified/Badgified.hpp>
#include <Geode/loader/GameEvent.hpp>
#include <hiimjasmine00.optional_settings/OptionalColor3BSetting.hpp>
#include <jasmine/setting.hpp>
#include <jasmine/web.hpp>

using namespace dasshu;
using namespace geode::prelude;
using namespace optional_settings;

std::map<int, int> developerBadges;

constexpr std::array ids = {
    "mod-developer"_spr,
    "verified-developer"_spr,
    "index-staff"_spr,
    "lead-developer"_spr
};

constexpr std::array names = {
    "Mod Developer",
    "Verified Developer",
    "Index Staff",
    "Lead Developer"
};

constexpr std::array descriptions = {
    "This user is a <ca>mod developer</c> for <cy>Geode</c>. "
    "They have created mods that are available on the <cy>Geode mod index</c>. "
    "They will have to have new mods and mod updates approved by the <cd>index staff</c>.",
    "This user is a <cp>verified mod developer</c> for <cy>Geode</c>. "
    "They can update their mods on the <cy>Geode mod index</c> without the need for approval. "
    "They will still have to have new mods approved by the <cd>index staff</c>.",
    "This user is a member of the <cd>index staff</c> for <cy>Geode</c>. "
    "They can approve or reject mods uploaded to the <cy>Geode mod index</c>.",
    "This user is a <co>lead developer</c> of <cy>Geode</c>. "
    "They are part of the main development team and have significant contributions to the <cy>Geode ecosystem</c>."
};

constexpr std::array settings = {
    std::make_pair("mod-developer-color", "mod-developer-color-toggle"),
    std::make_pair("verified-developer-color", "verified-developer-color-toggle"),
    std::make_pair("index-staff-color", "index-staff-color-toggle"),
    std::make_pair("lead-developer-color", "lead-developer-color-toggle")
};

$on_mod(Loaded) {
    auto mod = Mod::get();
    auto& data = mod->getSavedSettingsData();
    if (!mod->setSavedValue("migrated-colors", true)) {
        for (auto [key, toggle] : settings) {
            if (auto setting = jasmine::setting::get<std::optional<ccColor3B>>(key)) {
                auto oldColorValue = data.get<ccColor3B>(key);
                auto oldColorEnabled = data.get<bool>(toggle);
                if (oldColorValue.isOk() && oldColorEnabled.isOk()) {
                    setting->setStoredValue(oldColorValue.unwrap());
                    setting->setEnabled(oldColorEnabled.unwrap());
                }
            }
        }
    }

    for (int i = 0; i < ids.size(); i++) {
        badgified::registerBadge(
            ids[i],
            names[i],
            descriptions[i],
            [type = i + 1](const badgified::Badge& badge) {
                auto badgeIt = developerBadges.find(badge.user->m_accountID);
                if (badgeIt == developerBadges.end()) return;

                auto badgeType = badgeIt->second;
                if (badgeType != type) return;

                showBadge(badge, CCSprite::createWithSpriteFrameName(fmt::format("badge{:02}.png"_spr, badgeType).c_str()));
            }
        );

        auto commentColor = jasmine::setting::getValue<std::optional<ccColor3B>>(settings[i].first);
        if (commentColor.has_value()) badgified::setCommentColor(ids[i], commentColor.value());

        SettingChangedEventV3(GEODE_MOD_ID, settings[i].first).listen([id = ids[i]](std::shared_ptr<SettingV3> setting) {
            // TODO: Wait for Dasshu to add a way to set comment color to nullopt
            if (auto commentColor = std::static_pointer_cast<OptionalColor3BSetting>(std::move(setting))->getValue()) {
                badgified::setCommentColor(id, commentColor.value());
            }
            else if (badgified::getCommentColor(id).isOk()) {
                badgified::setCommentColor(id, { 255, 255, 255 });
            }
        }).leak();
    }
}

$on_game(Loaded) {
    spawn(
        web::WebRequest().get("https://badges.hiimjasmine00.com/developer"),
        [](web::WebResponse res) {
            if (!res.ok()) return;

            for (auto& value : jasmine::web::getArray(res)) {
                auto id = value.get<int>("id");
                if (!id.isOkAnd([](int id) { return id > 0; })) continue;

                auto type = value.get<int>("badge");
                if (!type.isOkAnd([](int type) { return type > 0 && type < 5; })) continue;

                developerBadges.emplace(id.unwrap(), type.unwrap());
            }
        }
    );
}
