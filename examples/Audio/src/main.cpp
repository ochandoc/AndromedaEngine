#include <assert.h>

#include <stdio.h>
#include <iostream>
#include <fstream>

#include <functional>
#include <algorithm>
#include <utility>

#include <optional>
#include <memory>

#include <string>
#include <vector>
#include <queue>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

#include "Common/JobSystem.h"
#include "Common/Engine.h"
#include "Common/Window.h"
#include "Common/GraphicsContext.h"
#include "Common/Renderer.h"
#include "Common/Shader.h"
#include "Common/Triangle.h"
#include "Common/Input.h"
#include "Common/ActionInput.h"
#include "Common/EntityComponentSystem.h"
#include "Common/AudioManager.h"
#include "Common/Audio.h"
#include "Common/Song.h"
#include "Common/Timer.h"



#include "Common/JobSystem.h"

#include <Windows.h>

LARGE_INTEGER StartTime;
LARGE_INTEGER Freq;

void ResetTimer()
{
    QueryPerformanceCounter(&StartTime);
}

void CheckTimer(const char* Name)
{
    static LARGE_INTEGER CurrentTime;
    QueryPerformanceCounter(&CurrentTime);

    printf("%s: %fms\n", Name, (((float)(CurrentTime.QuadPart - StartTime.QuadPart) / (float)Freq.QuadPart) * 1000.0f));
}

int select_num(int i)
{
  printf("Num selected\n");
  return i;
}

int select_num1(int i)
{
  std::this_thread::sleep_for(std::chrono::seconds(2));
  printf("Num selected 1\n");
  return i;
}

int select_num2(int i)
{
  std::this_thread::sleep_for(std::chrono::seconds(5));
  printf("Num selected 2\n");
  return i;
}

void print_value(int i, int a, int b)
{
  printf("Num: %d\n", i + a + b);
}


void DrawTriangle(And::Renderer& r, And::Triangle* tri){
  r.draw_triangle(tri);
}



int main(int argc, char** argv){
  QueryPerformanceFrequency(&Freq);
  srand(time(NULL));
  And::Engine e;

  And::JobSystem js;

  And::future<int> f{ 10 };
  And::future<int> f1{ 20 };
  And::future<int> f2{ 30 };

  And::future<int> future1 = js.add_job(select_num, f);
  And::future<int> future2 = js.add_job(select_num1, f1);
  And::future<int> future3 = js.add_job(select_num2, f2);

  js.add_job(print_value, future1, future2, future3);

  std::shared_ptr<And::Window> window = And::Window::make(e, 1024, 720, "Andromeda Engine");
  std::shared_ptr<And::GraphicsContext> g_context = window->get_context();
  And::Renderer g_renderer(*window);

  // Show pc info
  g_context->create_info();


  float clear_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  g_renderer.set_clear_color(clear_color);


  And::Input input{*window};

  And::ActionInput jump{"Jump", And::KeyState::Press, { And::KeyCode::L, And::KeyCode::V }};

  double mouseX = -1.0f, mouseY = -1.0f;
  double mouseXx = -1.0f, mouseYy = -1.0f;


  And::AudioManager audio_manager;

  And::Audio pepe_stereo;
  And::Audio pepe_mono;
  And::Audio door;
  And::Audio buscan_casa;
  pepe_stereo.load("../../data/audio/pepe_stereo.wav", "Pepe stereo");
  pepe_mono.load("../../data/audio/pepe_mono.wav", "Pepe mono");
  door.load("../../data/audio/door.wav", "Door");
  buscan_casa.load("../../data/audio/buscan_casa.wav", "Fin y Jake");



  std::vector<And::Audio*> eye_of_tiger;
  And::Audio drums;
  And::Audio guitar;
  And::Audio rytm;
  And::Audio song;
  drums.load("../../data/eye_of_tiger/drums.wav", "Drums");
  guitar.load("../../data/eye_of_tiger/guitar.wav", "Guitar");
  rytm.load("../../data/eye_of_tiger/rhythm.wav", "Rhythm");
  song.load("../../data/eye_of_tiger/song.wav", "Vocal");
  eye_of_tiger.push_back(&drums);
  eye_of_tiger.push_back(&guitar);
  eye_of_tiger.push_back(&rytm);
  eye_of_tiger.push_back(&song);

  And::Song rocky{eye_of_tiger, &audio_manager, "Eye Of The Tigger"};





  //audio.load("../../data/");

  //audio_manager.play(audio);


  float speed = 0.01f;

  Timer timer;

  while (window->is_open()){
    window->update();
    g_renderer.new_frame();
    audio_manager.Update();

    float dt = timer.GetDeltaTime();

  
    audio_manager.show_imgui(pepe_stereo);
    audio_manager.show_imgui(pepe_mono);
    audio_manager.show_imgui(door);
    audio_manager.show_imgui(buscan_casa);

    if (audio_manager.isPlaying(pepe_mono)) {
        float pepe_mono_position[3];
        float pepe_mono_velocity[3];

        pepe_mono.GetPosition(pepe_mono_position);
        pepe_mono.GetVelocity(pepe_mono_velocity);

        pepe_mono_position[0] += pepe_mono_velocity[0] * dt;
        pepe_mono_position[1] += pepe_mono_velocity[1] * dt;
        pepe_mono_position[2] += pepe_mono_velocity[2] * dt;

        pepe_mono.SetPosition(pepe_mono_position);
    }

    rocky.show_imgui();
    //rocky.play();

    //input.update_input();
    
    g_renderer.end_frame();
    window->swap_buffers();
  }


  
  

  
  return 0;
}