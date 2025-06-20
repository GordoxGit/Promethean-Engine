#pragma once

#include "ui/Widget.h"
#include "core/EventBus.h"
#include "assets/AssetManager.h"
#include "core/LogSystem.h"
#include <string>
#include <memory>
#include <SDL.h>
#include <glm/vec2.hpp>

using Promethean::AssetManager;
using Promethean::Texture;

/// Public state enumeration used for testing
enum class ButtonState { NORMAL, HOVER, PRESSED };

/** Event published when a button is clicked. */
struct ButtonClickedEvent {
    std::string buttonId;
};

/**
 * @brief Interactive button widget with three visual states.
 */
class Button : public Widget {
public:
    /**
     * @brief Construct a button with textures for each state.
     * @param id Identifier of the button.
     * @param textureNormal Path to the normal state texture.
     * @param textureHover Path to the hover state texture.
     * @param texturePressed Path to the pressed state texture.
     */
    Button(Promethean::AssetManager& assets,
           const std::string& id,
           const std::string& textureNormal,
           const std::string& textureHover,
           const std::string& texturePressed);

    void Draw(BatchRenderer& renderer) override;
    bool HandleEvent(const SDL_Event& event) override;
    void SetPosition(const glm::vec2& pos) override;
    void SetSize(const glm::vec2& size) override;

#ifdef TESTING
    ButtonState GetState() const { return static_cast<ButtonState>(m_state); }
#endif

private:
    Promethean::AssetManager& m_assets;
    std::string m_id;
    std::shared_ptr<Promethean::Texture> m_texNormal, m_texHover, m_texPressed;
    enum class State { NORMAL, HOVER, PRESSED } m_state{State::NORMAL};
    SDL_Rect m_bounds{0,0,100,50};
};

