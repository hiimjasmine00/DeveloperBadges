#include "../DeveloperBadges.hpp"
#include <Geode/binding/GameLevelManager.hpp>
#include <Geode/binding/GJComment.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/binding/GJUserScore.hpp>
#include <Geode/binding/TextArea.hpp>
#include <Geode/modify/CommentCell.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class $modify(DBCommentCell, CommentCell) {
    struct Fields {
        EventListener<web::WebTask> m_listener;
    };

    void loadFromComment(GJComment* comment) {
        CommentCell::loadFromComment(comment);

        auto badge = DeveloperBadges::badgeForUser(comment->m_accountID);
        if (badge.id != 0) return createBadge(badge);

        DeveloperBadges::loadBadge(std::move(m_fields->m_listener), comment->m_accountID, [this](const DeveloperBadge& badge) { createBadge(badge); });
    }

    void createBadge(const DeveloperBadge& badge) {
        auto usernameMenu = m_mainLayer->getChildByIDRecursive("username-menu");
        if (!usernameMenu) return;

        auto badgeSprite = CCSprite::createWithSpriteFrameName(fmt::format("badge{:02}.png"_spr, (int)badge.badge).c_str());
        badgeSprite->setScale(0.7f);
        auto badgeButton = CCMenuItemSpriteExtra::create(badgeSprite, this, menu_selector(DBCommentCell::onBadge));
        badgeButton->setID("developer-badge"_spr);
        badgeButton->setTag((int)badge.badge);
        badgeButton->setUserObject("badge-name"_spr, CCString::create(badge.name));
        usernameMenu->addChild(badgeButton);
        usernameMenu->updateLayout();

        auto savedLevel = GameLevelManager::get()->getSavedLevel(m_comment->m_levelID);
        if (m_accountComment || m_comment->m_modBadge > 0 || (savedLevel && savedLevel->m_userID == m_comment->m_userID)) return;
        if (badge.badge == BadgeType::ModDeveloper && !Mod::get()->getSettingValue<bool>("mod-developer-color-toggle")) return;
        if (badge.badge == BadgeType::VerifiedDeveloper && !Mod::get()->getSettingValue<bool>("verified-developer-color-toggle")) return;
        if (badge.badge == BadgeType::IndexStaff && !Mod::get()->getSettingValue<bool>("index-staff-color-toggle")) return;
        if (badge.badge == BadgeType::LeadDeveloper && !Mod::get()->getSettingValue<bool>("lead-developer-color-toggle")) return;

        if (auto commentTextLabel = static_cast<CCLabelBMFont*>(m_mainLayer->getChildByID("comment-text-label")))
            commentTextLabel->setColor(DeveloperBadges::getCommentColor(badge.badge));

        if (auto commentEmojisLabel = static_cast<CCLabelBMFont*>(m_mainLayer->getChildByID("thesillydoggo.comment_emojis/comment-text-label")))
            commentEmojisLabel->setColor(DeveloperBadges::getCommentColor(badge.badge));

        if (auto commentTextArea = static_cast<TextArea*>(m_mainLayer->getChildByID("comment-text-area")))
            commentTextArea->colorAllCharactersTo(DeveloperBadges::getCommentColor(badge.badge));

        if (auto commentEmojisArea = m_mainLayer->getChildByID("thesillydoggo.comment_emojis/comment-text-area")) {
            for (auto i = 0; i < commentEmojisArea->getChildrenCount(); i++) {
                if (auto child = typeinfo_cast<CCLabelBMFont*>(commentEmojisArea->getChildren()->objectAtIndex(i)))
                    child->setColor(DeveloperBadges::getCommentColor(badge.badge));
            }
        }
    }

    void onBadge(CCObject* sender) {
        auto badgeName = static_cast<CCString*>(static_cast<CCNode*>(sender)->getUserObject("badge-name"_spr));
        DeveloperBadges::showBadgeInfo(badgeName ? badgeName->getCString() : m_comment->m_userScore->m_userName, (BadgeType)sender->getTag());
    }
};
