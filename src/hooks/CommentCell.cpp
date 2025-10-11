#include "../DeveloperBadges.hpp"
#include <Geode/binding/GameLevelManager.hpp>
#include <Geode/binding/GJComment.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/binding/TextArea.hpp>
#include <Geode/modify/CommentCell.hpp>
#include <hiimjasmine00.optional_settings/include/OptionalColor3BSetting.hpp>

using namespace geode::prelude;

class $modify(DBCommentCell, CommentCell) {
    struct Fields {
        std::string m_badgeName;
    };

    static void onModify(ModifyBase<ModifyDerive<DBCommentCell, CommentCell>>& self) {
        (void)self.setHookPriorityBeforePost("CommentCell::loadFromComment", "thesillydoggo.comment_emojis");
        (void)self.setHookPriorityBeforePost("CommentCell::loadFromComment", "prevter.comment_emojis");
    }

    void loadFromComment(GJComment* comment) {
        CommentCell::loadFromComment(comment);

        auto badge = DeveloperBadges::badgeForUser(comment->m_accountID);
        if (!badge) return;

        auto usernameMenu = m_mainLayer->getChildByIDRecursive("username-menu");
        if (!usernameMenu) return;

        m_fields->m_badgeName = badge->name;

        auto badgeType = badge->type;
        auto badgeSprite = CCSprite::createWithSpriteFrameName(fmt::format("badge{:02}.png"_spr, badgeType).c_str());
        badgeSprite->setScale(0.7f);
        auto badgeButton = CCMenuItemSpriteExtra::create(badgeSprite, this, menu_selector(DBCommentCell::onBadge));
        badgeButton->setID("developer-badge"_spr);
        badgeButton->setTag(badgeType);
        usernameMenu->addChild(badgeButton);
        usernameMenu->updateLayout();

        if (m_accountComment || comment->m_modBadge > 0) return;
        if (auto savedLevel = GameLevelManager::get()->getSavedLevel(comment->m_levelID)) {
            if (savedLevel->m_userID == comment->m_userID) return;
        }

        constexpr std::array colors = {
            "mod-developer-color",
            "verified-developer-color",
            "index-staff-color",
            "lead-developer-color"
        };

        if (badgeType < 1 || badgeType > colors.size()) return;

        if (auto commentColor = Mod::get()->getSettingValue<std::optional<cocos2d::ccColor3B>>(colors[badgeType - 1])) {
            auto color = *commentColor;

            if (auto commentTextLabel = static_cast<CCLabelBMFont*>(m_mainLayer->getChildByID("comment-text-label"))) {
                commentTextLabel->setColor(color);
            }

            if (auto commentTextArea = static_cast<TextArea*>(m_mainLayer->getChildByID("comment-text-area"))) {
                commentTextArea->setColor(color);
                commentTextArea->colorAllCharactersTo(color);
            }
        }
    }

    void onBadge(CCObject* sender) {
        DeveloperBadges::showBadgeInfo(m_fields->m_badgeName, sender->getTag());
    }
};
