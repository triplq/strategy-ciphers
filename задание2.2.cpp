#include <iostream>
#include <fstream>
#include <filesystem>

class Strategy{
public:
    virtual ~Strategy() = default;
    virtual void encrypt(std::string&, int) = 0;
};

class Getting_strat{
public:
    virtual ~Getting_strat() = default;
    virtual void gettingBy(std::string&, int&) = 0;
};

class Caesar : public Strategy{
public:
    void encrypt(std::string &message, int key) override{
        std::string result{""};
        for (char ch : message) {
            if (isalpha(ch)) {
                char base = isupper(ch) ? 'A' : 'a';
                result += (char)(((ch - base + key) % 26) + base);
            } else {
                result += ch;
            }
        }
        message = result;
    };
};

class Atbash : public Strategy{
public:
    void encrypt(std::string& message, int key = 0) override {
        std::string result{""};
        for (char c : message) {
            if (isalpha(c)) {
                if (islower(c)) {
                    result += (char)('z' - (c - 'a'));
                } else {
                    result += (char)('Z' - (c - 'A'));
                }
            } else {
                result += c;
            }
        }
        message = result;
    };
};

class Vernam : public Strategy{
public: 
    void encrypt(std::string &message, int key) override{
        std::string rand_key_word{""};
        for (size_t i = 0; i < key; ++i) {
            rand_key_word += (char)(rand() % 256);
        }

        std::string result{""};
        for (size_t i = 0; i < message.length(); ++i) {
            result += (char)(message[i] ^ rand_key_word[i]);
        }
        
        message = result;
    };
};

class Encrypting_prog{
private:
    Strategy *strategy_;
    Getting_strat *get_strat_;
    std::string message;
    int key;

public:
    Encrypting_prog(Strategy *strategy, Getting_strat *get_st) : strategy_{strategy}, get_strat_{get_st} { };

    void setGetting(Getting_strat *get_st){
        delete this->get_strat_;
        this->get_strat_ = get_st;
    };

    void setEncrypt(Strategy *strategy){
        delete this->strategy_;
        this->strategy_ = strategy;
    };

    void getUtils(){
        this->get_strat_->gettingBy(this->message, this->key);
    };

    void doEncrypt(){
        if(strategy_ || message.size() == 0){
            this->strategy_->encrypt(this->message, this->key);
            std::cout << this->message << '\n';
        }
        else
            std::cout << "No strategy or message\n";
    };
};

class gettingByCons : public Getting_strat{
public:
    void gettingBy(std::string &message, int &key) override {
        std::cout << "Insert a message: "; std::cin >> message;
        std::cout << "Insert a key (it won't interfere if it's not necessary): "; std::cin >> key;
    };
};

class gettingByFile : public Getting_strat{
public:
    void gettingBy(std::string &message, int &key) override {
    std::string name;
    while(true){
        std::cout << "Enter a file name: "; std::cin >> name;
        if(!std::filesystem::exists(name))
            std::cout << "no such file\n";
        else
            break;
    }

    std::ifstream in;
    in.open(name);
    in >> message;
    in >> key;
};
};

int main(){
    Encrypting_prog *encrypting = new Encrypting_prog(new Caesar, new gettingByFile);

    encrypting->getUtils();
    encrypting->doEncrypt();

    encrypting->setGetting(new gettingByCons);
    encrypting->doEncrypt();

    return 0;
};