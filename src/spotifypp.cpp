#include "spotifypp.h"
#include "base64.hpp"
#include "httplib.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

namespace spotifypp {


void credentials::convert_to_base64() {
  string encoded_id;
  encoded_id = this->client_id + ":" + this->secret_id;
  this->base64_id = base64::to_base64(encoded_id);
}
void credentials::get_access_n_refresh() {
  httplib::Client cli("https://accounts.spotify.com");
  httplib::Headers headers = {{"Authorization", "Basic " + this->base64_id},
                              {"User-Agent", "curl/8.9.1"}};
  std::string body = "grant_type=authorization_code&code=" + this->auth_token +
                     "&redirect_uri=http://localhost:8888/callback";
  httplib::Result res = cli.Post("/api/token", headers, body,
                                 "application/x-www-form-urlencoded");
  cout << res->body << endl;
  nlohmann::json codes = nlohmann::json::parse(res->body);
  this->access_token = codes["access_token"];
  this->refresh_token = codes["refresh_token"];
};
string credentials::get_auth_url() {
  string url =
      "https://accounts.spotify.com/authorize?client_id=" + this->client_id +
      "&response_type=code&redirect_uri=http://localhost:8888/"
      "callback&scope=user-read-playback-state&user-modify-playback-state&"
      "state=1235";
  return url;
}
void credentials::create_auth_url()
{
  string url =
  "https://accounts.spotify.com/authorize?client_id=" + this->client_id +
  "&response_type=code&redirect_uri=http://localhost:8888/"
  "callback&scope=user-read-playback-state&user-modify-playback-state&"
  "state=12345";
    this->auth_url = url;

}
void credentials::get_auth() {
  string authorization_code;
  httplib::Server svr;
  svr.Get("/callback",
          [&](const httplib::Request &req, httplib::Response &res) {
            res.set_content("Go back to the app!", "text/plain");
            authorization_code = req.get_param_value("code");
            svr.stop();
          });
  cout << this->auth_url << endl;
  svr.listen("127.0.0.1", 8888);
  this->auth_token = authorization_code;
}

void credentials::create_config(string path) {
  ofstream config(path);
  nlohmann::json file;
  file["client_id"] = this->client_id;
  file["encoded_id"] = this->base64_id;
  file["refresh_token"] = this->refresh_token;
  config << file;
  cout << "Created config file" << endl;
  config.close();
}
void credentials::read_config(string path) {
  ifstream config(path);
  nlohmann::json file = nlohmann::json::parse(config);
  this->refresh_token = file["refresh_token"];
  this->base64_id = file["encoded_id"];
  this->client_id = file["client_id"];
}
void credentials::use_refresh_token() {
  httplib::Client cli("https://accounts.spotify.com");
  httplib::Headers headers = {{"User-Agent", "curl/8.9.1"},
                              {"Authorization", "Basic " + this->base64_id}};
  std::string body =
      "grant_type=refresh_token&refresh_token=" + this->refresh_token +
      "&client_id=" + this->client_id;
  httplib::Result res = cli.Post("/api/token", headers, body,
                                 "application/x-www-form-urlencoded");
  this->access_token = nlohmann::json::parse(res->body)["access_token"];
}
song::song(string access_token) {
  this->access_token = access_token;
  get_song();
};
void song::get_song() {
  httplib::Client cli("https://api.spotify.com");
  httplib::Headers headers = {
      {"User-Agent", "curl/8.9.1"},
      {"Authorization", "Bearer " + this->access_token}};
  httplib::Result res = cli.Get("/v1/me/player/currently-playing", headers);
  if (res->body.empty()) {
    cout << "Spotify is not opened" << endl;
  } else {
    nlohmann::json content = nlohmann::json::parse(res->body);
    this->song_name = content["item"]["name"];
    this->album_name = content["item"]["album"]["name"];
    this->artist_name = content["item"]["album"]["artists"][0]["name"];
    this->album_url = content["item"]["album"]["images"][2]["url"];
    this->song_uri = content["item"]["uri"];
    this->is_playing = content["is_playing"];
    
    this->album_data = get_album_cover();
    cout << res->body << endl;
  }
}
string song::get_album_cover() {
  httplib::Client cli("https://i.scdn.co");
  httplib::Headers headers = {
      {"User-Agent", "curl/8.9.1"},
  };
  string delimiter = ".co";
  size_t pos = this->album_url.find(delimiter);
  std::string result = this->album_url.substr(pos + delimiter.length());

  httplib::Result res = cli.Get(result, headers);
  return res->body;
}

void credentials::skip_forward()
{
  httplib::Headers headers = {
      {"User-Agent", "curl/8.9.1"},
      {"Authorization", "Bearer " + this->access_token}};
  httplib::Client cli("https://api.spotify.com");
  httplib::Result res = cli.Post("/v1/me/player/next", headers);
}
void credentials::skip_backward()
{
  httplib::Headers headers = {{"User-Agent", "curl/8.9.1"},
                              {"Authorization", "Bearer " + this->access_token}};
  httplib::Client cli("https://api.spotify.com");
  httplib::Result res = cli.Post("/v1/me/player/previous", headers);
}
void credentials::pause()
{
    httplib::Client cli("https://api.spotify.com");
    cli.set_default_headers({
        {"User-Agent", "curl/8.9.1"},
        {"Authorization", "Bearer " + this->access_token}
    });
    httplib::Result res = cli.Put("/v1/me/player/pause", "", "application/json");
  }

  void credentials::unpause()
{
    httplib::Client cli("https://api.spotify.com");
    cli.set_default_headers({
        {"User-Agent", "curl/8.9.1"},
        {"Authorization", "Bearer " + this->access_token}
    });
    httplib::Result res = cli.Put("/v1/me/player/play", "{}", "application/json");
  }
  void credentials::set_id(string client_id, string secret_id)
  {
    this->client_id = client_id;
    this->secret_id = secret_id;
  }
  // even tho you can get the song lenght from the currently playing, skipping songs will cause not knowing what the hell is currently playing
  int credentials::get_song_lenght()
  {

  }
}; // namespace spotifypp
