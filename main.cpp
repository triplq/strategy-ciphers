#include <iostream>

class Strategy{
public:
    virtual ~Strategy() = default;
    virtual void encrypt(std::string&, int) = 0;
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
    std::string message;
    int key;

public:
    Encrypting_prog(Strategy *strategy) : strategy_(strategy) { };

    void setMessage(const std::string& msg) { this->message = msg; };
    void setKey(const int &k) { this->key = k; };

    void setEncrypt(Strategy *strategy){
        delete this->strategy_;
        this->strategy_ = strategy;
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

int main(){
    Encrypting_prog *encrypting = new Encrypting_prog(new Caesar);
    encrypting->setKey(2);
    encrypting->setMessage("Alexey");
    encrypting->doEncrypt();

    encrypting->setEncrypt(new Atbash);
    encrypting->setMessage("Caesar");
    encrypting->doEncrypt();

    return 0;
};