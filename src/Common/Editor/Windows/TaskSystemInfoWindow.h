#pragma once

#include "Andromeda/Editor/EditorWindow.h"
#include "Andromeda/HAL/Window.h"

#include "imgui.h"

namespace And
{
	struct ScrollingBuffer {
		int MaxSize;
		int Offset;
		ImVector<ImVec2> Data;
		ScrollingBuffer(int max_size = 5000) {
			MaxSize = max_size;
			Offset = 0;
			Data.reserve(MaxSize);
		}
		void AddPoint(float x, float y) {
			if (Data.size() < MaxSize)
				Data.push_back(ImVec2(x, y));
			else {
				Data[Offset] = ImVec2(x, y);
				Offset = (Offset + 1) % MaxSize;
			}
		}
		void Erase() {
			if (Data.size() > 0) {
				Data.shrink(0);
				Offset = 0;
			}
		}
	};

  struct TaskDisplayInfo
  {
		size_t ExecutionThread;
		size_t Id;
		std::source_location SourceLocation;
		float Time;
		std::string FunctionString;
		std::string TaskName;
  };

  struct WorkerDisplayInfo
  {
		std::string Name;
    size_t NumTasks = 0;
		ScrollingBuffer buff;
  };

  class TaskSystemInfoWindow : public EditorWindow
  {
  public:
    TaskSystemInfoWindow();

    virtual ~TaskSystemInfoWindow();

    virtual void Show() override;

		void AddWorker(const std::string& name, size_t Id);
		void UpdateNumTasks(size_t Id, size_t num_tasks);
		size_t GetNumTasks(size_t Id);
		void AddTaskInProgressInfo(TaskDisplayInfo& taskInfo);
		void RemoveTaskInProgressInfo(size_t Id);
		void AddTaskCompletedInfo(TaskDisplayInfo& taskInfo);
		void AddTaskWaitingInfo(TaskDisplayInfo& taskInfo);
		void RemoveTaskWaitingInfo(size_t Id);

  private:
		std::unordered_map<size_t, WorkerDisplayInfo> m_Workers;
		std::unordered_map<size_t, TaskDisplayInfo> m_InProgressTasks;
		std::unordered_map<size_t, TaskDisplayInfo> m_CompletedTasks;
		std::unordered_map<size_t, TaskDisplayInfo> m_WaitingTasks;
  };
}