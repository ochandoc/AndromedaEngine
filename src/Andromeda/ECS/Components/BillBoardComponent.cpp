#include "Andromeda/ECS/Components/BillBoardComponent.h"

namespace And {

    BillBoardComponent::BillBoardComponent() = default;

    BillBoardComponent::BillBoardComponent(const BillBoardComponent& other) :
        m_bill_board(other.m_bill_board), m_active(other.m_active) {

        for (int i = 0; i < 3; i++) {
            bScale[i] = other.bScale[i];
        }

    }

    BillBoardComponent::BillBoardComponent(BillBoardComponent&& other) noexcept :
        m_bill_board(std::move(other.m_bill_board)), m_active(other.m_active) {

        for (int i = 0; i < 3; i++) {
            bScale[i] = other.bScale[i];
        }
    }

    BillBoardComponent::~BillBoardComponent() = default;

    BillBoardComponent& BillBoardComponent::operator=(const BillBoardComponent& other) {
        if (this != &other) {
            m_bill_board = other.m_bill_board;
            m_active = other.m_active;
            for (int i = 0; i < 3; i++) {
                bScale[i] = other.bScale[i];
            }
        }
        return *this;
    }

    BillBoardComponent& BillBoardComponent::operator=(BillBoardComponent&& other) noexcept {
        if (this != &other) {
            m_bill_board = std::move(other.m_bill_board);
            m_active = other.m_active;
            for (int i = 0; i < 3; i++) {
                bScale[i] = other.bScale[i];
            }
        }
        return *this;
    }

}