#pragma once

class DeltaTime {
private:
    DeltaTime() = default;
    ~DeltaTime() = default;
public:
    static DeltaTime& get_instance() {
        static DeltaTime instance;
        return instance;
    }
    static void instantiate() {
        get_instance().delta_time = 0.0f;
    }
    static void update(double now) {
        get_instance().delta_time = now - get_instance().last_time;
        get_instance().last_time = now;
    }
    static float get() {
        return get_instance().delta_time;
    }
private:
    float delta_time;
    float last_time;
};
