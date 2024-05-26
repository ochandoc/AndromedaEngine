#include "Andromeda/Graphics/BillBoard.h"

namespace And {

    std::shared_ptr<And::MeshComponent> BillBoard::m_Mesh = nullptr;

    BillBoard::BillBoard() {
        if (!m_Mesh) {
            m_Mesh = std::make_shared<And::MeshComponent>();
            m_Mesh->MeshOBJ = And::Geometry::load("cube.obj");
            And::RawMesh raw_mesh(m_Mesh->MeshOBJ->get_vertices(), m_Mesh->MeshOBJ->get_indices());
            std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh);
            m_Mesh->SetMesh(mesh);
        }
    }

    BillBoard::BillBoard(const BillBoard& other) :
        m_material(other.m_material){
    }

    BillBoard::BillBoard(BillBoard&& other) noexcept :
        m_material(std::move(other.m_material)) {

    }

    BillBoard::~BillBoard() {
    }

    BillBoard& BillBoard::operator=(const BillBoard& other) {
        if (this != &other) {
            m_material = other.m_material;
        }
        return *this;
    }

    BillBoard& BillBoard::operator=(BillBoard&& other) noexcept {
        if (this != &other) {
            m_material = std::move(other.m_material);
        }
        return *this;
    }

}