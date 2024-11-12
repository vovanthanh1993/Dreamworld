#include "DialogueBox.h"
#include <iostream> // Thư viện cho việc in ra console, thay thế bằng thư viện đồ họa bạn sử dụng

// Constructor
DialogueBox::DialogueBox(float x, float y, float width, float height)
    : m_position(x, y), m_size(width, height), m_text("") {}

// Phương thức để vẽ khung hội thoại
void DialogueBox::Draw() {
    // Ví dụ: Vẽ hình chữ nhật tại m_position với kích thước m_size
    std::cout << "Drawing dialogue box at (" << m_position.x << ", " << m_position.y
        << ") with size (" << m_size.x << ", " << m_size.y << ")." << std::endl;

    // Vẽ khung hội thoại bằng thư viện đồ họa của bạn ở đây
    // Ví dụ: sử dụng một hàm từ thư viện đồ họa
}

// Phương thức để thiết lập văn bản
void DialogueBox::SetText(const std::string& text) {
    m_text = text;
}

// Phương thức để hiển thị khung hội thoại
void DialogueBox::Show() {
    Draw();
    // Vẽ văn bản lên khung hội thoại
    std::cout << "Text: " << m_text << std::endl;
    // Thay thế bằng phương thức vẽ văn bản từ thư viện đồ họa của bạn
}
