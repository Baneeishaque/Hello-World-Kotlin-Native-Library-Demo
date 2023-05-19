#include "flutter_window.h"

#include <flutter/event_channel.h>
#include <flutter/event_sink.h>
#include <flutter/event_stream_handler_functions.h>
#include <flutter/method_channel.h>
#include <flutter/standard_method_codec.h>
#include <windows.h>

#include <memory>
#include <optional>

#include "../../../Hello-World-Library-Kotlin-Native-Gradle/build/bin/native/debugShared/native_api.h"

#include "flutter/generated_plugin_registrant.h"

static constexpr int kBatteryError = -1;
static constexpr int kNoBattery = -2;

static int GetBatteryLevel() {
  SYSTEM_POWER_STATUS status;
  if (GetSystemPowerStatus(&status) == 0) {
    return kBatteryError;
  } else if (status.BatteryFlag == 128) {
    return kNoBattery;
  } else if (status.BatteryLifePercent == 255) {
    return kBatteryError;
  }
  return status.BatteryLifePercent;
}

FlutterWindow::FlutterWindow(const flutter::DartProject& project)
    : project_(project) {}

FlutterWindow::~FlutterWindow() {}

bool FlutterWindow::OnCreate() {
  if (!Win32Window::OnCreate()) {
    return false;
  }

  RECT frame = GetClientArea();

  // The size here must match the window dimensions to avoid unnecessary surface
  // creation / destruction in the startup path.
  flutter_controller_ = std::make_unique<flutter::FlutterViewController>(
      frame.right - frame.left, frame.bottom - frame.top, project_);
  // Ensure that basic setup of the controller was successful.
  if (!flutter_controller_->engine() || !flutter_controller_->view()) {
    return false;
  }
  RegisterPlugins(flutter_controller_->engine());

  flutter::MethodChannel<> channel(
          flutter_controller_->engine()->messenger(), "samples.flutter.io/battery",
          &flutter::StandardMethodCodec::GetInstance());
  channel.SetMethodCallHandler(
          [](const flutter::MethodCall<>& call,
             std::unique_ptr<flutter::MethodResult<>> result) {
              if (call.method_name() == "getBatteryLevel") {
                int battery_level = GetBatteryLevel();

                if (battery_level == kBatteryError) {
                  result->Error("UNAVAILABLE", "Battery level not available.");
                } else if (battery_level == kNoBattery) {
                  result->Error("NO_BATTERY", "Device does not have a battery.");
                } else {
                  result->Success(battery_level);
                }
              } else if (call.method_name() == "getComputationResult") {
                  
                  native_ExportedSymbols* lib = native_symbols();

                  native_kref_example_Clazz newInstance = lib->kotlin.root.example.Clazz.Clazz();
                  int x = lib->kotlin.root.example.Clazz.memberFunction(newInstance, 42);
                  lib->DisposeStablePointer(newInstance.pinned);
                  
                  result->Success(x);

              } else {
                result->NotImplemented();
              }
          });

  SetChildContent(flutter_controller_->view()->GetNativeWindow());

  flutter_controller_->engine()->SetNextFrameCallback([&]() {
    this->Show();
  });

  return true;
}

void FlutterWindow::OnDestroy() {
  if (flutter_controller_) {
    flutter_controller_ = nullptr;
  }

  Win32Window::OnDestroy();
}

LRESULT
FlutterWindow::MessageHandler(HWND hwnd, UINT const message,
                              WPARAM const wparam,
                              LPARAM const lparam) noexcept {
  // Give Flutter, including plugins, an opportunity to handle window messages.
  if (flutter_controller_) {
    std::optional<LRESULT> result =
        flutter_controller_->HandleTopLevelWindowProc(hwnd, message, wparam,
                                                      lparam);
    if (result) {
      return *result;
    }
  }

  switch (message) {
    case WM_FONTCHANGE:
      flutter_controller_->engine()->ReloadSystemFonts();
      break;
  }

  return Win32Window::MessageHandler(hwnd, message, wparam, lparam);
}
