require("Assets\\Scripts\\bootstrap");

Test = class(BaseComponent,
{
});

function Test:Initialize()
    print("Car Initializing");
end

function Test:Update(deltaTime)
    local position = self.Game:GetPosition();

    print("position.x = " .. position.x);
end