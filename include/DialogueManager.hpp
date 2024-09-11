#pragma once

#include <string>
#include <queue>
#include <functional>
#include <chrono>
#include <utility>
#include <emscripten.h>

class DialogueData {
public:
    std::string text;
    std::string image;
    std::string sound;
    int32_t     delay_ms;
    std::function<void()> on_end;

    DialogueData(
            const std::string& text,
            const std::string& image,
            const std::string& sound,
            const int32_t& delay_ms,
            const std::function<void()>& on_end) :
        text(text), image(image), sound(sound), delay_ms(delay_ms),
        on_end(on_end) {
    }
};

class DialogueManager {
public:

    void add_dialogue(
            const std::string &text,
            const std::string &image,
            const std::string &sound,
            const int32_t& delay_ms,
            const std::function<void()>& on_end = nullptr) {
        m_dialogues.emplace(text, image, sound, delay_ms, on_end);
    }

    void start() {
        if (!m_is_running && !m_dialogues.empty()) {
            m_is_running = true;
            m_last_dialogue_time = std::chrono::steady_clock::now();
            next_dialogue();
        }
    }

    void update() {
        if (m_is_running) {
            auto now = std::chrono::steady_clock::now();
            std::chrono::duration<float> elapsed = now - m_last_dialogue_time;

            if (elapsed.count() * 1000 >= m_current_delay) {
                if (m_on_end) m_on_end();
                if (!m_dialogues.empty()) {
                    next_dialogue();
                } else {
                    EM_ASM({
                        document.getElementById("hero-dialogue").style.display = "none";
                    });
                    m_is_running = false;
                }
            }
        }
    }

private:
    void next_dialogue() {
        if (m_dialogues.empty()) {
            m_is_running = false;
            EM_ASM({
                document.getElementById("hero-dialogue").style.display = "none";
            });
            return;
        }

        auto dialogue = m_dialogues.front();
        m_dialogues.pop();
        m_current_delay = dialogue.delay_ms;
        m_last_dialogue_time = std::chrono::steady_clock::now();
        m_on_end = dialogue.on_end;
        show_hero_dialogue(dialogue.text, dialogue.image, dialogue.sound);
    }

    void show_hero_dialogue(const std::string &text, const std::string &image, const std::string &sound) {
        EM_ASM({
            var dialogueElement = document.getElementById('hero-dialogue');
            var bubbleText = dialogueElement.querySelector('.dialogue-bubble p');
            var dialogueImg = dialogueElement.querySelector('img');
            var heroSound = document.getElementById(UTF8ToString($2));

            bubbleText.textContent = UTF8ToString($0);
            dialogueImg.src = UTF8ToString($1);
            dialogueElement.style.display = 'block';

            heroSound.play();
        }, text.c_str(), image.c_str(), sound.c_str());
    }

    std::queue<DialogueData> m_dialogues;
    bool m_is_running = false;
    int32_t m_current_delay = 0; // Задержка текущего диалога
    std::chrono::steady_clock::time_point m_last_dialogue_time;
    std::function<void()> m_on_end;
};
