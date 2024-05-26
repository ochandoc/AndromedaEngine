#pragma once
#include "Andromeda/ECS/Components/MaterialComponent.h"
#include "Andromeda/ECS/Components/MeshComponent.h"

namespace And {

    
    class BillBoard {
    public:
        BillBoard();
        BillBoard(const BillBoard&);
        BillBoard(BillBoard&&) noexcept;
        ~BillBoard();

        BillBoard& operator=(const BillBoard&);
        BillBoard& operator=(BillBoard&&) noexcept;

        void SetMaterial(std::shared_ptr <MaterialComponent> mat) { m_material = mat; };
        std::shared_ptr<MaterialComponent> GetMaterialComponent() { return m_material; }

        MeshComponent* GetMeshComponent() { return m_Mesh.get(); }

    private:
        static std::shared_ptr<And::MeshComponent> m_Mesh;
        std::shared_ptr<MaterialComponent> m_material;
    };

}