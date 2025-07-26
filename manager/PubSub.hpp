/**
 * Pub/Sub Manager - subscribe(), unsubscribe(), unsubscribeAll(), publish()
 *
 */
#ifndef REDIS_IN_MEMORY_PUBSUB_MANAGER_HPP
#define REDIS_IN_MEMORY_PUBSUB_MANAGER_HPP

#include <string>
#include <mutex>
#include <set>

using namespace std;

class PubSubManager
{
    // Constructor in Private
    PubSubManager() = default;
    ~PubSubManager() = default;

    // Channels Global Storage
    unordered_map<string, set<int>> channels;
    mutex mtx;

public:
    static PubSubManager &instance()
    {
        // Create and written instance
        static PubSubManager mgr;
        return mgr;
    }

    // Prevent from Copy Constructor
    PubSubManager(const PubSubManager &) = delete;
    PubSubManager &operator=(const PubSubManager &) = delete;
    PubSubManager(PubSubManager &&) = delete;
    PubSubManager &operator=(PubSubManager &&) = delete;

    // subscribe method
    void subscribe(const string &channel, int clientSok)
    {
        lock_guard<mutex> lock(mtx);

        // push channel to channels
        channels[channel].insert(clientSok);
    }

    int unsubscribe(const string& channel, int clientSok) {
        lock_guard<mutex> lock(mtx);
        
        // Find the channel and erase from set
        auto it = channels.find(channel);
        if (it == channels.end()) return 0;     // Channel not found
        it->second.erase(clientSok);
        int count = it->second.size();
        // Erase the Subscriber
        if(it->second.empty()) channels.erase(it);
        return count;
    }

    void unsubscribeAll(int clientSok) {
        // Unsubscribe the all 
        lock_guard<mutex> lock(mtx);

        // looping
        for (auto it = channels.begin(); it != channels.end();) {
            it->second.erase(clientSok);
            if(it->second.empty()) it = channels.erase(it);
            else it++;
        }
    }

    int subscriberCount(const string& channel) {
        lock_guard<mutex> lock(mtx);

        // Find the channel
        auto it = channels.find(channel);
        return it == channels.end() ? 0 : it->second.size();
    }

    int publish(const std::string& channel, const std::string& message) {
        // Double-locking: copy subscribers under lock, notify outside
        std::set<int> subs;
        {
            std::lock_guard<std::mutex> lock(mtx);
            auto it = channels.find(channel);
            if (it == channels.end()) return 0;
            subs = it->second;
        }
        int count = 0;
        for (int sock : subs) {
            std::string resp = "*3\r\n";
            resp += "$7\r\nmessage\r\n";
            resp += "$" + std::to_string(channel.size()) + "\r\n" + channel + "\r\n";
            resp += "$" + std::to_string(message.size()) + "\r\n" + message + "\r\n";
            send(sock, resp.data(), resp.size(), 0);
            ++count;
        }
        return count;
    }
};

#endif