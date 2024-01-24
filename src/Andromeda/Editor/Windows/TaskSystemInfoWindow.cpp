#include "TaskSystemInfoWindow.h"

#include "Common/UI/Plot/implot.h"

And::TaskSystemInfoWindow::TaskSystemInfoWindow() : EditorWindow("Task System Info")
{
	
}

And::TaskSystemInfoWindow::~TaskSystemInfoWindow()
{
}

void And::TaskSystemInfoWindow::Show()
{
	static float history = 10.0f;
	static float t = 0;

	t += m_Window->get_delta_time();

	for (auto& [id, worker] : m_Workers)
	{
		worker.buff.AddPoint(t, static_cast<float>(worker.NumTasks));
	}

	if (m_is_open)
	{
		if (ImGui::Begin("Task System Info", &m_is_open))
		{
			if (ImPlot::BeginPlot("##Task System Info Plot", ImVec2(-1, 150))) {
				ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, ImPlotAxisFlags_NoTickLabels);
				ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
				ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);
				ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);

				for (auto& [id, worker] : m_Workers)
				{
					ImPlot::PlotLine(worker.Name.c_str(), &worker.buff.Data[0].x, &worker.buff.Data[0].y, worker.buff.Data.size(), 0, worker.buff.Offset, 2 * sizeof(float));
				}

				ImPlot::EndPlot();
			}

			if (ImGui::BeginTabBar("##Task System Info Tasks"))
			{
				for (auto& [workerId, worker] : m_Workers)
				{
					if (ImGui::BeginTabItem(worker.Name.c_str()))
					{
						ImGui::Text("In Queue Tasks");
						ImGui::Separator();
						for (auto& [taskId, task] : m_InProgressTasks)
						{
							if (task.ExecutionThread == workerId)
							{
								std::string NodeStr = "[";
								NodeStr += std::to_string(taskId) + "] ";
								NodeStr += task.TaskName;
								if (ImGui::TreeNode(NodeStr.c_str()))
								{
									ImGui::BulletText("Id: %zu", task.Id);
									ImGui::BulletText("Name: %s", task.TaskName.c_str());
									ImGui::BulletText("Task Function: %s", task.FunctionString.c_str());
									NodeStr = std::string("Task Creation Info##") + NodeStr;
									if (ImGui::TreeNode(NodeStr.c_str()))
									{
										ImGui::BulletText("File Name: %s", task.SourceLocation.file_name());
										ImGui::BulletText("Function Name: %s", task.SourceLocation.function_name());
										ImGui::BulletText("Line: %u", task.SourceLocation.line());
										ImGui::TreePop();
									}
									ImGui::TreePop();
								}
							}
						}
						ImGui::Text("Completed Tasks");
						ImGui::Separator();
						for (auto& [taskId, task] : m_CompletedTasks)
						{
							if (task.ExecutionThread == workerId)
							{
								std::string NodeStr = "[";
								NodeStr += std::to_string(taskId) + "] ";
								NodeStr += task.TaskName;
								if (ImGui::TreeNode(NodeStr.c_str()))
								{
									ImGui::BulletText("Id: %zu", task.Id);
									ImGui::BulletText("Name: %s", task.TaskName.c_str());
									ImGui::BulletText("Time: %fs", task.Time);
									ImGui::BulletText("Task Function: %s", task.FunctionString.c_str());
									NodeStr = std::string("Task Creation Info##") + NodeStr;
									if (ImGui::TreeNode(NodeStr.c_str()))
									{
										ImGui::BulletText("File Name: %s", task.SourceLocation.file_name());
										ImGui::BulletText("Function Name: %s", task.SourceLocation.function_name());
										ImGui::BulletText("Line: %u", task.SourceLocation.line());
										ImGui::TreePop();
									}
									ImGui::TreePop();
								}
							}
						}
						ImGui::EndTabItem();
					}
				}

				if (ImGui::BeginTabItem("Waiting Tasks"))
				{
					for (auto& [taskId, task] : m_WaitingTasks)
					{
						std::string NodeStr = "[";
						NodeStr += std::to_string(taskId) + "] ";
						NodeStr += task.TaskName;
						if (ImGui::TreeNode(NodeStr.c_str()))
						{
							ImGui::BulletText("Id: %zu", task.Id);
							ImGui::BulletText("Name: %s", task.TaskName.c_str());
							ImGui::BulletText("Task Function: %s", task.FunctionString.c_str());
							NodeStr = std::string("Task Creation Info##") + NodeStr;
							if (ImGui::TreeNode(NodeStr.c_str()))
							{
								ImGui::BulletText("File Name: %s", task.SourceLocation.file_name());
								ImGui::BulletText("Function Name: %s", task.SourceLocation.function_name());
								ImGui::BulletText("Line: %u", task.SourceLocation.line());
								ImGui::TreePop();
							}
							ImGui::TreePop();
						}
					}
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}
		ImGui::End();
	}

}

void And::TaskSystemInfoWindow::AddWorker(const std::string& Name, size_t Id)
{
	WorkerDisplayInfo di;
	di.Name = Name;
	m_Workers.insert({ Id, di });
}

void And::TaskSystemInfoWindow::UpdateNumTasks(size_t Id, size_t num_tasks)
{
	m_Workers[Id].NumTasks = num_tasks;
}

size_t And::TaskSystemInfoWindow::GetNumTasks(size_t Id)
{
	return m_Workers[Id].NumTasks;
}

void And::TaskSystemInfoWindow::AddTaskInProgressInfo(TaskDisplayInfo& taskInfo)
{
	m_InProgressTasks.insert({taskInfo.Id, taskInfo});
}

void And::TaskSystemInfoWindow::RemoveTaskInProgressInfo(size_t Id)
{
	m_InProgressTasks.erase(Id);
}

void And::TaskSystemInfoWindow::AddTaskCompletedInfo(TaskDisplayInfo& taskInfo)
{
	m_CompletedTasks.insert({ taskInfo.Id, taskInfo });
}

void And::TaskSystemInfoWindow::AddTaskWaitingInfo(TaskDisplayInfo& taskInfo)
{
	m_WaitingTasks.insert({ taskInfo.Id, taskInfo });
}

void And::TaskSystemInfoWindow::RemoveTaskWaitingInfo(size_t Id)
{
	m_WaitingTasks.erase(Id);
}


