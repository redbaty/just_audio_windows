#include "include/just_audio_windows/just_audio_windows_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

#include "player.hpp"

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <sstream>

using flutter::EncodableMap;
using flutter::EncodableValue;

// Looks for |key| in |map|, returning the associated value if it is present, or
// a nullptr if not.
const EncodableValue* ValueOrNull(const EncodableMap& map, const char* key) {
  auto it = map.find(EncodableValue(key));
  if (it == map.end()) {
    return nullptr;
  }
  return &(it->second);
}

namespace {

static std::unordered_map<std::string, AudioPlayer> players;

class JustAudioWindowsPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  JustAudioWindowsPlugin();

  virtual ~JustAudioWindowsPlugin();

 private:
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
};

// static
void JustAudioWindowsPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {
  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "just_audio_windows",
          &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<JustAudioWindowsPlugin>();

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

JustAudioWindowsPlugin::JustAudioWindowsPlugin() {}

JustAudioWindowsPlugin::~JustAudioWindowsPlugin() {}

void JustAudioWindowsPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  const auto* args =std::get_if<flutter::EncodableMap>(method_call.arguments());
  if (args) {
    if (method_call.method_name().compare("init") == 0) {
      const auto* id = std::get_if<std::string>(ValueOrNull(*args, "id"));
      if (!id) {
        return result->Error("id_error", "id argument missing");
      }
      // AudioPlayer player { *id };
      // players.insert(std::pair<std::string, AudioPlayer>(*id, player));
      result->Success();
    } else if (method_call.method_name().compare("disposePlayer") == 0) {
      result->Success();
    } else if (method_call.method_name().compare("disposeAllPlayers") == 0) {
      result->Success();
    } else {
      result->NotImplemented();
    }
  } else {
    result->NotImplemented();
  }
}

}  // namespace

void JustAudioWindowsPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  JustAudioWindowsPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
