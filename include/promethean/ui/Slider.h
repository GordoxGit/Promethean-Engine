#pragma once

#include "ui/Widget.h"
#include "core/EventBus.h"
#include <SDL.h>
#include <string>
#include <glm/vec2.hpp>

/** Event fired when the slider value changes. */
struct SliderValueChangedEvent {
    std::string sliderId;
    float       newValue;   ///< Normalized [0.0, 1.0]
};

/**
 * @brief Horizontal slider widget with draggable knob.
 */
class Slider : public Widget {
public:
    Slider(const std::string& id,
           const std::string& trackTex,
           const std::string& knobTex,
           float initialValue = 0.5f);

    void Draw(BatchRenderer& renderer) override;
    bool HandleEvent(const SDL_Event& event) override;
    void SetPosition(const glm::vec2& pos) override;
    void SetSize(const glm::vec2& size) override;

    void SetValue(float value);      ///< Set normalized value [0,1]
    float GetValue() const;

#ifdef TESTING
    const SDL_Rect& GetKnobRect() const { return m_knobRect; }
#endif

private:
    std::string m_id;
    float       m_value{0.f};
    SDL_Rect    m_trackRect{0,0,100,10};
    SDL_Rect    m_knobRect{0,0,10,10};
    uint32_t    m_trackTexId{0};
    uint32_t    m_knobTexId{0};
    bool        m_dragging{false};
    SDL_FingerID m_activeFinger{0};

    void UpdateFromPosition(int x);
};

