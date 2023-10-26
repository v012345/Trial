#include "GameLib/GameLib.h"
#include "GameLib/Input/Keyboard.h"
#include "GameLib/Input/Manager.h"
using namespace GameLib;

#include "Game/State.h"
#include "Pad.h"
#include "Sequence/Game/Clear.h"
#include "Sequence/Game/Failure.h"
#include "Sequence/Game/Judge.h"
#include "Sequence/Game/Parent.h"
#include "Sequence/Game/Pause.h"
#include "Sequence/Game/Play.h"

namespace Sequence {
    namespace Game {

        Play::Play() { // 什么都不做
        }

        Play::~Play() { // 什么都不做
        }

        // 游戏本体
        Base* Play::update(Parent* parent) {
            Base* next = this;
            Input::Keyboard kb = Input::Manager::instance().keyboard();

            State* state = parent->state();

            bool cleared = state->hasCleared();
            bool die1P = !state->isAlive(0);
            bool die2P = !state->isAlive(1);
            // 使用debug命令行测试。
            if (kb.isTriggered('1')) { // 2P杀人
                die2P = true;
            } else if (kb.isTriggered('2')) { // 1P杀人
                die1P = true;
            } else if (kb.isTriggered('c')) {
                cleared = true;
            }
            // 如果按空格键，则暂停
            // 清除后报告
            if (parent->mode() == Parent::MODE_1P) {
                if (cleared && !die1P) {
                    parent->goToNextStage();
                    next = new Clear;
                } else if (die1P) {
                    parent->reduceLife();
                    next = new Failure;
                }
            } else { // 双人
                if (die1P || die2P) { // 如果任何一方死亡，则判断胜负
                    next = new Judge;
                    if (die1P && die2P) {
                        parent->setWinner(Parent::PLAYER_NONE); // 都死了
                    } else if (die1P) {
                        parent->setWinner(Parent::PLAYER_2);
                    } else {
                        parent->setWinner(Parent::PLAYER_1);
                    }
                }
            }
            if ((next == this) && Pad::isTriggered(Pad::B)) { // 用B按钮暂停。不过只有当你还没有动的时候
                next = new Pause;
            }
            // 更新
            state->update();
            // 绘制
            state->draw();

            return next;
        }

    } // namespace Game
} // namespace Sequence
