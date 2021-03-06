#pragma once

#include <SDL2/SDL.h>

#include "AnimatedSprite.h"
#include "FrameAnimation.h"

class Spaceship: public AnimatedSprite{
    public:
        Spaceship(): AnimatedSprite("Sprites/Spaceship/spaceship", 38, 20, 200, 500){
            flame = new AnimatedSprite("Sprites/Flame/flame", 4, 15);
            explosion = new FrameAnimation("Sprites/Explosions/Explosion0/explosion", 23, 1, 15);
            shield = new Sprite("Sprites/shield.png");

            leftGunCharge = 0;
            rightGunCharge = 0;

            shielded = false;
        }

        ~Spaceship(){
            delete flame;
            delete explosion;
            delete shield;
        }

        void Update(double dt){
            if (leftGunCharge >= 1){
                leftGunCharge -= dt * 75;
            }
            
            if (rightGunCharge >= 1){
                rightGunCharge -= dt * 75;
            }

            AnimatedSprite::Update(dt);
            flame->SetPosition(position.first, position.second+60);
            flame->Update(dt);

            explosion->Update();

            shield->SetPosition(position.first-20, position.second);
            shield->Update();
        }

        void Draw(){
            SDL_Rect leftGun;
            leftGun.x = position.first - 7;
            leftGun.y = position.second;
            leftGun.w = 2;
            leftGun.h = leftGunCharge / 500 * height;
            SDL_FillRect(Helpers::surface, &leftGun, SDL_MapRGB(Helpers::surface->format, 255, 73, 0));
            
            SDL_Rect rightGun;
            rightGun.x = position.first + width + 7;
            rightGun.y = position.second;
            rightGun.w = 2;
            rightGun.h = rightGunCharge / 500 * height;
            SDL_FillRect(Helpers::surface, &rightGun, SDL_MapRGB(Helpers::surface->format, 255, 73, 0));

            AnimatedSprite::Draw();
            flame->Draw();
            explosion->Draw(Helpers::surface, position.first-20, position.second);

            if (shielded){
                shield->Draw();
            }
        }

        AnimatedSprite *flame;
        FrameAnimation *explosion;
        Sprite *shield;

        double leftGunCharge;
        double rightGunCharge;

        bool shielded;
};
