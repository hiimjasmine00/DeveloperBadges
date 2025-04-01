#include "../DeveloperBadges.hpp"
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class $modify(DBMenuLayer, MenuLayer) {
    inline static bool triedLoading = false;

    bool init() override {
        if (!MenuLayer::init()) return false;

        if (triedLoading) return true;
        triedLoading = true;

        static std::optional<web::WebTask> task;
        task = web::WebRequest().get(BADGES_URL).map([](web::WebResponse* res) {
            if (!res->ok() || !res->json().isOk()) return task = std::nullopt, *res;

            auto badges = res->json().unwrapOr(std::vector<matjson::Value>());
            if (!badges.isArray()) return task = std::nullopt, *res;

            DeveloperBadges::developerBadges = ranges::reduce<std::vector<DeveloperBadge>>(badges.asArray().unwrap(),
                [](std::vector<DeveloperBadge> & vec, const matjson::Value& value) {
                    if (value.contains("id") && value["id"].isNumber() && value["id"].asInt().unwrap() > 0
                        && value.contains("name") && value["name"].isString()
                        && value.contains("badge") && value["badge"].isNumber() && value["badge"].asInt().unwrap() > 0) vec.push_back({
                            .id = (int)value["id"].asInt().unwrap(),
                            .name = value["name"].asString().unwrap(),
                            .badge = (BadgeType)value["badge"].asInt().unwrap()
                        });
                });

            return task = std::nullopt, *res;
        });

        return true;
    }
};
