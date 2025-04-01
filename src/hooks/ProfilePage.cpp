#include "../DeveloperBadges.hpp"
#include <Geode/binding/GJUserScore.hpp>
#include <Geode/modify/ProfilePage.hpp>

using namespace geode::prelude;

class $modify(DBProfilePage, ProfilePage) {
    struct Fields {
        bool m_hasBadge;
    };

    void loadPageFromUserInfo(GJUserScore* score) {
        ProfilePage::loadPageFromUserInfo(score);

        auto f = m_fields.self();
        if (f->m_hasBadge) return;

        auto badge = DeveloperBadges::badgeForUser(score->m_accountID);
        if (badge.id == 0) return;

        auto usernameMenu = m_mainLayer->getChildByID("username-menu");
        if (!usernameMenu) return;

        auto badgeButton = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName(fmt::format("badge{:02}.png"_spr, (int)badge.badge).c_str()),
            this, menu_selector(DBProfilePage::onBadge)
        );
        badgeButton->setID("developer-badge"_spr);
        badgeButton->setTag((int)badge.badge);
        badgeButton->setUserObject("badge-name"_spr, CCString::create(badge.name));
        usernameMenu->addChild(badgeButton);
        usernameMenu->updateLayout();

        f->m_hasBadge = true;
    }

    void onBadge(CCObject* sender) {
        auto badgeName = static_cast<CCString*>(static_cast<CCNode*>(sender)->getUserObject("badge-name"_spr));
        DeveloperBadges::showBadgeInfo(badgeName ? badgeName->m_sString : m_score->m_userName, (BadgeType)sender->getTag());
    }
};
