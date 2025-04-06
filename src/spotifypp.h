#ifndef SPOTIFYAUTH_H
#define SPOTIFYAUTH_H


#include <string>
#include <httplib.h>

using namespace std;

namespace spotifypp {
class credentials {
    public:
        string access_token;
        string refresh_token;
        string auth_url;
        string get_auth_url();
        void get_auth();
        void read_config(string path);
        void use_refresh_token();
        void get_access_n_refresh();
        void convert_to_base64();
        void create_config(string path);
        void create_auth_url();
        void set_id(string client_id, string secret_id);
        int get_song_lenght();
        string client_id;
        string secret_id;
        string auth_token;
        string base64_id;
        void skip_forward();
        void skip_backward();
        void pause();
        void unpause();
        
    };

    class song
    {
        public:
            song() = default;
            string song_name;
            string album_name;
            string artist_name;
            string album_url;
            string album_data;
            string song_uri;
            // gets the total lenght of the song
            int song_lenght;
            // gets where the song is at
            int song_current_lenght;
            bool is_playing;
            song(string access_token);
            string get_album_cover();
            void get_song();
        private:
            string access_token;
    };
};

#endif //SPOTIFYAUTH_H