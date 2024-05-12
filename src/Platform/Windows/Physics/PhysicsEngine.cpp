#include "Andromeda/Physics/PhysicsEngine.h"


#define PX_PHYSX_STATIC_LIB 1
#include "PxPhysicsAPI.h"
#include "Andromeda/ECS/Components/TransformComponent.h"




namespace And {

struct PhysicsData {
	physx::PxPhysics* physics;
	physx::PxRigidDynamic* actor;
	physx::PxScene* scene;
};
struct PhysicsEngineData {

		PhysicsEngineData() : default_allocator_callback(), error_callback(), cpu_dispatcher(nullptr),
							  tolerance_scale(), foundation(nullptr), physics(nullptr), scene(nullptr),
							  material(nullptr), pvd(nullptr), transport(nullptr), client(nullptr){}

		physx::PxDefaultAllocator			default_allocator_callback;
		physx::PxDefaultErrorCallback		error_callback;
		physx::PxDefaultCpuDispatcher*		cpu_dispatcher = nullptr; // Multithreading
		physx::PxTolerancesScale			tolerance_scale; // How process colliders

		physx::PxFoundation*				foundation = nullptr; // Settings
		
		physx::PxPhysics*					physics = nullptr; // Core


		physx::PxScene*						scene = nullptr;
		physx::PxMaterial*					material = nullptr;
		
		physx::PxPvd*						pvd = nullptr;
		physx::PxPvdTransport*				transport = nullptr;
		physx::PxSceneDesc*					sceneDesc; // Esto no era un puntero
		physx::PxPvdSceneClient*			client;
	};

std::shared_ptr<PhysicsEngine> PhysicsEngine::Init(bool executeOnGPU, unsigned int substeps, bool create_plane){
	
	std::shared_ptr<PhysicsEngine> engine(new PhysicsEngine);
	engine->m_physics_data = std::make_shared<PhysicsEngineData>();

	engine->m_physics_data->foundation = PxCreateFoundation(PX_PHYSICS_VERSION, engine->m_physics_data->default_allocator_callback, engine->m_physics_data->error_callback);
	if (!engine->m_physics_data->foundation) {
		printf("\n*** Error creating physics foundation ***\n");
		return nullptr;
	}

	engine->m_physics_data->pvd = PxCreatePvd(*(engine->m_physics_data->foundation));
	engine->m_physics_data->transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10); // ip, port, timeout
	engine->m_physics_data->pvd->connect(*(engine->m_physics_data->transport), physx::PxPvdInstrumentationFlag::eALL);

	engine->m_physics_data->tolerance_scale.length = 1; // typical lenght of an object
	engine->m_physics_data->tolerance_scale.speed = 981; // speed of and object in earth gravity

	physx::PxFoundation* foundation_tmp;
	physx::PxTolerancesScale scale_tmp;
	
	engine->m_physics_data->physics = PxCreateBasePhysics(PX_PHYSICS_VERSION, *(engine->m_physics_data->foundation), engine->m_physics_data->tolerance_scale, true, engine->m_physics_data->pvd);
	if (engine->m_physics_data->physics == nullptr) {
		printf("\n*** Error creating physics ***\n");
		return nullptr;
	}

	// Gravity
	(engine->m_physics_data->sceneDesc) = new physx::PxSceneDesc(engine->m_physics_data->physics->getTolerancesScale());
	engine->m_physics_data->sceneDesc->gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);

	// Sub steps (physics quality)
	engine->m_physics_data->sceneDesc->nbContactDataBlocks = substeps;

	// Threads
	engine->m_physics_data->cpu_dispatcher = physx::PxDefaultCpuDispatcherCreate(2); // Num threads
	engine->m_physics_data->sceneDesc->cpuDispatcher = engine->m_physics_data->cpu_dispatcher;

	engine->m_physics_data->sceneDesc->filterShader = physx::PxDefaultSimulationFilterShader;

	// Flags
	engine->m_physics_data->sceneDesc->flags |= physx::PxSceneFlag::eENABLE_PCM;

	if (executeOnGPU) {
		engine->m_physics_data->sceneDesc->flags |= physx::PxSceneFlag::eENABLE_GPU_DYNAMICS;
	}
	//engine->m_physics_data->sceneDesc->flags |= physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS;
	//engine->m_physics_data->sceneDesc->flags |= physx::PxSceneFlag::eEXCLUDE_KINEMATICS_FROM_ACTIVE_ACTORS;

	// Estabiliza rigid bodys pero puedes perder el momentum (confirmado que si)
	//engine->m_physics_data->sceneDesc->flags |= physx::PxSceneFlag::eENABLE_STABILIZATION;


	//engine->m_physics_data->sceneDesc->flags |= physx::PxSceneFlag::eENABLE_ACTIVETRANSFORMS;


	engine->m_physics_data->scene = engine->m_physics_data->physics->createScene(*(engine->m_physics_data->sceneDesc));
	if (!engine->m_physics_data->scene) {
		printf("\n*** Error creating scene ***\n");
		return nullptr;
	}


	/*engine->m_physics_data->client = engine->m_physics_data->scene->getScenePvdClient();
	if (!engine->m_physics_data->client) {
		printf("\n*** Error creating physics client ***\n");
		return nullptr;
	}else {
		engine->m_physics_data->client->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		engine->m_physics_data->client->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		engine->m_physics_data->client->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}*/

	// Create simulation
	if (create_plane) {
		engine->m_physics_data->material = engine->m_physics_data->physics->createMaterial(0.6f, 0.6f, 0.7f); // static friction, dynamic friction, restitution
		physx::PxRigidStatic* groundPlane = PxCreatePlane(*(engine->m_physics_data->physics), physx::PxPlane(0.0f, 1.0f, 0.0f, 1.0f), *(engine->m_physics_data->material));
		engine->m_physics_data->scene->addActor(*groundPlane);
	}



	
	
	return engine;
}

PhysicsEngine::PhysicsEngine() : m_physics_data(){

}

PhysicsEngine::~PhysicsEngine(){

}

PhysicsEngine::PhysicsEngine(PhysicsEngine&& other){

	this->m_physics_data = other.m_physics_data;
}

void PhysicsEngine::GetError(){

	//m_physics_data->error_callback.reportError
}

RigidBody PhysicsEngine::CreateRigidBody(){
	RigidBody rb{};
	PhysicsData data;
	data.physics = m_physics_data->physics;
	data.scene = m_physics_data->scene;

	rb.m_data = std::make_shared<PhysicsData>(data);
	return rb;
}

void PhysicsEngine::Simulate(double dt, bool fetch) {
	m_physics_data->scene->simulate(dt);

	// Advanced split method (for complex scenes)
	//const physx::PxContactPairHeader* pairHeader;
	//physx::PxU32 nbContactPairs;
	//m_physics_data->scene->fetchResultsStart(pairHeader, nbContactPairs, true);
	//Set up continuation task to be run after callbacks have been processed in parallel
	//callbackFinishTask.setContinuation(*gScene->getTaskManager(), NULL);
	//callbackFinishTask.reset();
	//process the callbacks
	//gScene->processCallbacks(&callbackFinishTask);
	//callbackFinishTask.removeReference();
	//callbackFinishTask.wait();
	//gScene->fetchResultsFinish();

	m_physics_data->scene->fetchResults(fetch);
}

void PhysicsEngine::Apply(EntityComponentSystem& ecs) {

	for (auto [tr, rb] : ecs.get_components<TransformComponent, RigidBody>()) {
		rb->GetPositionRotation(tr->position, tr->rotation);
	}
}

void PhysicsEngine::Release(EntityComponentSystem& ecs){
	for (auto [rb] : ecs.get_components<RigidBody>()) {
		rb->Release();
	}
}


void PhysicsEngine::SetGravity(float x, float y, float z) {

	// Iterar sobre todos los rigid body y llamar a wakeup()
	m_physics_data->scene->setGravity(physx::PxVec3(x,y,z));
}

void PhysicsEngine::SetObjectLenght(unsigned int l) {

}

void PhysicsEngine::SetSpeedScale(unsigned int speed) {

}

}