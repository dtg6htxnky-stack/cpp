#include <iostream>
#include <string>
#include <map>

class ConfigManager {
public:
    ConfigManager(const ConfigManager& other) = delete;
    ConfigManager& operator=(const ConfigManager& other) = delete;

    static ConfigManager& instance() {
        static ConfigManager c;
        return c;
    }

    void set(const std::string& key, const std::string& value) {
        data[key] = value;
    }

    std::string get(const std::string& key) {
        return data[key];
    }

private:
    ConfigManager() {
        std::cout << "ConfigManager constructed\n";
    }
    std::map<std::string, std::string> data;
};

class Widget {
public:
    Widget() {
        liveCount += 1;
    }

    ~Widget() {
        liveCount -= 1;
    }

    static int count() {
        return liveCount;
    }
    
private:
    static int liveCount;
};

int Widget::liveCount = 0;

int main(){
  ConfigManager::instance().set("mode", "prod");
  std::cout << ConfigManager::instance().get("mode") << "\n";
  ConfigManager::instance();
  std::cout << Widget::count() << "\n";
  {
    Widget a, b;
    std::cout << Widget::count() << "\n";
    Widget c;
    std::cout << Widget::count() << "\n";
  }
  std::cout << Widget::count() << "\n";
}