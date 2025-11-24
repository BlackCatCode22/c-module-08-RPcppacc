// dH 11/20/24
// chatBot01.cpp

#include <iostream>
#include <string>
#include <C:/Program Files/JetBrains/CLion 2025.2/bin/mingw/curl/curl-8.11.0_2-win64-mingw/include/curl/curl.h>
#include "C:/Users/repen/OneDrive/Desktop/chatBot01/include/nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

// Callback function for cURL response
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* out) {
    size_t totalSize = size * nmemb;
    out->append((char*)contents, totalSize);
    return totalSize;
}

// Function to fetch the current time in Italy from WorldTimeAPI
string getTimeInItaly() {
    string responseString;
    CURL* curl = curl_easy_init();

    if (curl) {
        string url = "https://worldtimeapi.org/api/timezone/Europe/Rome";

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

        // Use a CA certificate bundle for SSL verification
        curl_easy_setopt(curl, CURLOPT_CAINFO, "C:/Users/repen/OneDrive/Desktop/chatBot01/cacert.pem");

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cerr << "cURL error: " << curl_easy_strerror(res) << endl;
        }

        curl_easy_cleanup(curl);
    }

    if (responseString.empty()) {
        return "Error: Could not fetch the time. Please try again later.";
    }

    // Parse the JSON response
    try {
        json jsonResponse = json::parse(responseString);
        if (jsonResponse.contains("datetime")) {
            return jsonResponse["datetime"];
        } else {
            return "Error: Unexpected response format from WorldTimeAPI.";
        }
    } catch (const json::exception& e) {
        return "Error: Failed to parse response from WorldTimeAPI.";
    }
}

// Function to send a message to OpenAI API
string sendMessageToChatbot(const string& userMessage, const string& apiKey) {
    string responseString;
    CURL* curl = curl_easy_init();

    if (curl) {
        string url = "https://api.openai.com/v1/chat/completions";
        string payload = R"({
            "model": "gpt-3.5-turbo",
            "messages": [{"role": "user", "content": ")" + userMessage + R"("}]
        })";

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

        // Use a CA certificate bundle for SSL verification
        curl_easy_setopt(curl, CURLOPT_CAINFO, "C:/Users/repen/OneDrive/Desktop/chatBot01/cacert.pem");

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cerr << "cURL error: " << curl_easy_strerror(res) << endl;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    if (responseString.empty()) {
        cerr << "Error: Empty response from API." << endl;
        return "";
    }

    return responseString;
}

int main() {
    string apiKey = "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";
    string userMessage;
    string chatbotName = "Assistant";  // Default chatbot name
    string userName = "User";          // Default user name

    cout << "Chatbot (type 'exit' to quit):/n";
    while (true) {
        cout << "> ";
        getline(cin, userMessage);

        if (userMessage == "exit") break;

        // Handle "name assignment" logic
        if (userMessage.find("Your name is now") != string::npos) {
            size_t start = userMessage.find("Your name is now") + 17;
            chatbotName = userMessage.substr(start);
            chatbotName.erase(chatbotName.find_last_not_of(" /n/r/t") + 1);  // Trim whitespace
            cout << "Bot: Okay, I will now call myself " << chatbotName << ". What else can I do for you?" << endl;
            continue;
        }
        if (userMessage.find("my name is") != string::npos) {
            size_t start = userMessage.find("my name is") + 10;
            userName = userMessage.substr(start);
            userName.erase(userName.find_last_not_of(" /n/r/t") + 1);  // Trim whitespace
            cout << chatbotName << ": Nice to meet you, " << userName << "! How can I assist you today?" << endl;
            continue;
        }

        // Handle "time in Italy" queries directly
        if (userMessage.find("time in Italy") != string::npos) {
            string timeInItaly = getTimeInItaly();
            cout << chatbotName << ": The current time in Italy is: " << timeInItaly << "/n";
            continue;
        }

        // Send other queries to OpenAI API
        string response = sendMessageToChatbot(userMessage, apiKey);

        try {
            json jsonResponse = json::parse(response);

            // Check if the response has the expected structure
            if (jsonResponse.contains("choices") && !jsonResponse["choices"].empty()) {
                if (jsonResponse["choices"][0]["message"].contains("content") && !jsonResponse["choices"][0]["message"]["content"].is_null()) {
                    string chatbotReply = jsonResponse["choices"][0]["message"]["content"];
                    cout << chatbotName << ": " << chatbotReply << "/n";
                } else {
                    cerr << chatbotName << ": Sorry, I didn't understand the response. Please try again." << endl;
                }
            } else {
                cerr << chatbotName << ": Sorry, I couldn't get a valid response from the server." << endl;
            }
        } catch (const json::exception& e) {
            cerr << chatbotName << ": JSON parsing error: " << e.what() << "/n";
        }
    }

    return 0;
}
