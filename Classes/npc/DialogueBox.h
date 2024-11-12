#ifndef DIALOGUEBOX_H
#define DIALOGUEBOX_H

#include <box2d/box2d.h>
#include <string>

class DialogueBox {
public:
    // Constructor
    DialogueBox(float x, float y, float width, float height);

    // Phương thức để vẽ khung hội thoại
    void Draw();

    // Phương thức để thiết lập văn bản
    void SetText(const std::string& text);

    // Phương thức để hiển thị khung hội thoại
    void Show();

private:
    b2Vec2 m_position; // Vị trí của khung hội thoại
    b2Vec2 m_size;     // Kích thước của khung hội thoại
    std::string m_text; // Nội dung văn bản
};

#endif // DIALOGUEBOX_H