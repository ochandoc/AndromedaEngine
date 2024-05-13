#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"

#include "Andromeda/ECS/ComponentBase.h"

namespace And
{
  struct Mat4;

  class TransformComponent : public ComponentBase{

  public:
    float position[3] = { 0.0f };
    float rotation[4] = { 0.0f };
    float scale[3] = { 1.0f };
    //float padding[2] = { 0.0f };

    TransformComponent();
    ~TransformComponent();
    TransformComponent(const TransformComponent&);
    TransformComponent(TransformComponent&&);
    TransformComponent operator=(const TransformComponent&);


    TransformComponent* m_parent = nullptr;

    // hijo  * padre


    float* GetModelMatrix();
    void SetParent(TransformComponent* parent);
    void SetPosition(float* p);
    void SetPosition(float x, float y, float z);

    void SetRotation(float* r);
    void SetRotation(float x, float y, float z);

    void SetScale(float* t);
    void SetScale(float x, float y, float z);

    void HasRigidBody(bool value = true) { m_has_rb_ = value; }

    void Reset();
  private:
      bool m_should_recalculate = true;
    
      //float* m_model_matrix = nullptr;
      std::shared_ptr<Mat4> m_matrix;
      bool m_has_rb_ = false;
  };
}