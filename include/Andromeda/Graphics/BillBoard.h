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

        /**
          * @brief Sets the material component.
          *
          * @param mat A shared pointer to the material component.
          */
        void SetMaterial(std::shared_ptr<MaterialComponent> mat) { m_material = mat; }

        /**
         * @brief Retrieves the material component.
         *
         * @return A shared pointer to the material component.
         */
        std::shared_ptr<MaterialComponent> GetMaterialComponent() { return m_material; }

        /**
         * @brief Retrieves the mesh component.
         *
         * @return A pointer to the mesh component.
         */
        MeshComponent* GetMeshComponent() { return m_Mesh.get(); }

    private:
        static std::shared_ptr<And::MeshComponent> m_Mesh;
        std::shared_ptr<MaterialComponent> m_material;
    };

}