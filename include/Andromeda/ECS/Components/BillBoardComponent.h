#pragma once
#include "Andromeda/Graphics/BillBoard.h"
#include "Andromeda/ECS/ComponentBase.h"

namespace And {

    class BillBoardComponent : public ComponentBase {
    public:
        BillBoardComponent();
        BillBoardComponent(const BillBoardComponent&);
        BillBoardComponent(BillBoardComponent&&) noexcept;
        ~BillBoardComponent();

        BillBoardComponent& operator=(const BillBoardComponent&);
        BillBoardComponent& operator=(BillBoardComponent&&) noexcept;

        void SetBillBoard(std::shared_ptr<BillBoard> bill) { m_bill_board = bill; }
        std::shared_ptr<BillBoard> GetBillBoard() { return m_bill_board; }

        void ActiveBillBoard(bool active) { m_active = active; }
        bool GetActive() { return m_active; };

        float bScale[3] = { 1.0f, 1.0f, 0.01f };

    private:
        std::shared_ptr<BillBoard> m_bill_board;
        bool m_active = true;

    };

}