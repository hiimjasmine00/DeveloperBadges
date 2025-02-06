#include "../DeveloperBadges.hpp"
#include <Geode/binding/GJUserScore.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/ranges.hpp>

using namespace geode::prelude;

class $modify(DBProfilePage, ProfilePage) {
    struct Fields {
        EventListener<web::WebTask> m_listener;
        CCMenuItemSpriteExtra* m_badgeButton;
        bool m_hasBadge;
    };

    void loadPageFromUserInfo(GJUserScore* score) {
        ProfilePage::loadPageFromUserInfo(score);

        auto f = m_fields.self();
        if (f->m_hasBadge) return;

        auto badge = DeveloperBadges::badgeForUser(score->m_accountID);
        if (badge.id != 0) return createBadge(badge);

        DeveloperBadges::loadBadge(std::move(f->m_listener), score->m_accountID, [this](const DeveloperBadge& badge) { createBadge(badge); });
    }

    void onUpdate(CCObject* sender) {
        ProfilePage::onUpdate(sender);

        DeveloperBadges::LOADED_IDS.erase(m_score->m_accountID);
        ranges::remove(DeveloperBadges::DEVELOPER_BADGES, [this](const DeveloperBadge& badge) { return badge.id == m_score->m_accountID; });

        DeveloperBadges::loadBadge(std::move(m_fields->m_listener), m_score->m_accountID, [this](const DeveloperBadge& badge) {
            auto f = m_fields.self();
            if (!f->m_hasBadge || !f->m_badgeButton) return createBadge(badge);

            static_cast<CCSprite*>(f->m_badgeButton->getNormalImage())->setDisplayFrame(
                CCSpriteFrameCache::get()->spriteFrameByName(fmt::format("badge{:02}.png"_spr, (int)badge.badge).c_str()));
            f->m_badgeButton->setTag((int)badge.badge);
        });
    }

    void createBadge(const DeveloperBadge& badge) {
        auto usernameMenu = m_mainLayer->getChildByID("username-menu");
        if (!usernameMenu) return;

        auto f = m_fields.self();
        f->m_badgeButton = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName(fmt::format("badge{:02}.png"_spr, (int)badge.badge).c_str()),
            this, menu_selector(DBProfilePage::onBadge)
        );
        f->m_badgeButton->setID("developer-badge"_spr);
        f->m_badgeButton->setTag((int)badge.badge);
        f->m_badgeButton->setUserObject("badge-name"_spr, CCString::create(badge.name));
        usernameMenu->addChild(f->m_badgeButton);
        usernameMenu->updateLayout();

        f->m_hasBadge = true;
    }

    void onBadge(CCObject* sender) {
        DeveloperBadges::showBadgeInfo(static_cast<CCString*>(
            static_cast<CCNode*>(sender)->getUserObject("badge-name"_spr))->getCString(), (BadgeType)sender->getTag());
    }
};
