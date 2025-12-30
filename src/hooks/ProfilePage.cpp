/*
#include "../DeveloperBadges.hpp"
#include <Geode/binding/GJUserScore.hpp>
#include <Geode/modify/ProfilePage.hpp>

using namespace geode::prelude;

class $modify(DBProfilePage, ProfilePage) {
    struct Fields {
        std::string m_badgeName;
        bool m_hasBadge;
    };

    void loadPageFromUserInfo(GJUserScore* score) {
        ProfilePage::loadPageFromUserInfo(score);

        auto f = m_fields.self();
        if (f->m_hasBadge) return;

        auto badge = DeveloperBadges::badgeForUser(score->m_accountID);
        if (!badge) return;

        auto usernameMenu = m_mainLayer->getChildByID("username-menu");
        if (!usernameMenu) return;

        f->m_badgeName = badge->name;

        auto badgeType = badge->type;
        auto badgeButton = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName(fmt::format("badge{:02}.png"_spr, badgeType).c_str()),
            this, menu_selector(DBProfilePage::onBadge)
        );
        badgeButton->setID("developer-badge"_spr);
        badgeButton->setTag(badgeType);
        usernameMenu->addChild(badgeButton);
        usernameMenu->updateLayout();

        f->m_hasBadge = true;
    }

    void onBadge(CCObject* sender) {
        DeveloperBadges::showBadgeInfo(m_fields->m_badgeName, sender->getTag());
    }
};
*/