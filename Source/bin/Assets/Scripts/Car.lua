require("Assets\\Scripts\\bootstrap");

Car = class(BaseComponent,
{
    speed = 0;
    acceleration = 0;
});

function Car:Initialize()
    print("Car Initializing");
end

function Car:Update(deltaTime)
    local position = self.Game:GetPosition();

    --print("position.y = " .. position.y);
end