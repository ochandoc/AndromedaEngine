#include "Andromeda/TaskSystem/Future/FutureAvailability.h"

namespace And
{
  FutureAvailability::FutureAvailability(const FutureAvailability& other) { AvailabilityBase = other.AvailabilityBase; Id = other.Id; }
  FutureAvailability::FutureAvailability(FutureAvailability&& other) { AvailabilityBase = std::move(other.AvailabilityBase); std::swap(Id, other.Id); }

  FutureAvailability& FutureAvailability::operator=(const FutureAvailability& other)
  {
    if (this != &other)
    {
      AvailabilityBase = other.AvailabilityBase;
      Id = other.Id;
    }
    return *this;
  }

  FutureAvailability& FutureAvailability::operator=(FutureAvailability&& other)
  {
    if (this != &other)
    {
      AvailabilityBase = std::move(other.AvailabilityBase);
      std::swap(Id, other.Id);
    }
    return *this;
  }

  bool FutureAvailability::IsAvailable() const
  {
    return AvailabilityBase->IsAvailable();
  }

  size_t FutureAvailability::GetId() const { return Id; }
}