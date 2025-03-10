#include <iostream>
#include <sstream>
#include <string>

// Перечисление для хранения состояния устройств
enum switches {
    LIGHTS_INSIDE = 1,       // Внутренний свет
    LIGHTS_OUTSIDE = 2,      // Наружный свет
    HEATERS = 4,             // Отопление в помещении
    WATER_PIPE_HEATING = 8,  // Обогрев водопровода
    CONDITIONER = 16         // Кондиционер
};

// Функция для проверки, включено ли устройство
bool is_device_on(int state, switches device) {
    return (state & device) != 0;
}

// Функция для включения устройства
void turn_on(int& state, switches device) {
    if (!is_device_on(state, device)) {
        state |= device;
        switch (device) {
            case LIGHTS_INSIDE:
                std::cout << "Lights ON!" << std::endl;
                break;
            case LIGHTS_OUTSIDE:
                std::cout << "Outdoor lights ON!" << std::endl;
                break;
            case HEATERS:
                std::cout << "Heaters ON!" << std::endl;
                break;
            case WATER_PIPE_HEATING:
                std::cout << "Water pipe heating ON!" << std::endl;
                break;
            case CONDITIONER:
                std::cout << "Conditioner ON!" << std::endl;
                break;
        }
    }
}

// Функция для выключения устройства
void turn_off(int& state, switches device) {
    if (is_device_on(state, device)) {
        state &= ~device;
        switch (device) {
            case LIGHTS_INSIDE:
                std::cout << "Lights OFF!" << std::endl;
                break;
            case LIGHTS_OUTSIDE:
                std::cout << "Outdoor lights OFF!" << std::endl;
                break;
            case HEATERS:
                std::cout << "Heaters OFF!" << std::endl;
                break;
            case WATER_PIPE_HEATING:
                std::cout << "Water pipe heating OFF!" << std::endl;
                break;
            case CONDITIONER:
                std::cout << "Conditioner OFF!" << std::endl;
                break;
        }
    }
}

// Функция для вычисления цветовой температуры света
int calculate_color_temperature(int hour) {
    if (hour >= 16 && hour < 20) {
        // Плавное изменение цветовой температуры с 5000K до 2700K
        return 5000 - (hour - 16) * (5000 - 2700) / 4;
    } else {
        // В остальное время цветовая температура 5000K
        return 5000;
    }
}

int main() {
    int switches_state = 0;  // Начальное состояние всех устройств (все выключены)
    int hour = 0;            // Начальное время (00:00)
    int day = 1;             // Начальный день (день 1)

    // Симуляция работы умного дома на протяжении двух дней
    while (day <= 2) {
        std::cout << "Day " << day << ", Hour " << hour << ":00" << std::endl;
        std::cout << "Temperature inside, temperature outside, movement, lights: ";
        std::string input;
        std::getline(std::cin, input);
        std::stringstream ss(input);

        int temp_inside, temp_outside;
        std::string movement, lights;
        ss >> temp_inside >> temp_outside >> movement >> lights;

        // Управление обогревом водопровода
        if (temp_outside < 0) {
            turn_on(switches_state, WATER_PIPE_HEATING);
        } else if (temp_outside > 5) {
            turn_off(switches_state, WATER_PIPE_HEATING);
        }

        // Управление наружным светом
        if ((hour >= 16 || hour < 5) && movement == "yes") {
            turn_on(switches_state, LIGHTS_OUTSIDE);
        } else {
            turn_off(switches_state, LIGHTS_OUTSIDE);
        }

        // Управление отоплением в помещении
        if (temp_inside < 22) {
            turn_on(switches_state, HEATERS);
        } else if (temp_inside >= 25) {
            turn_off(switches_state, HEATERS);
        }

        // Управление кондиционером
        if (temp_inside >= 30) {
            turn_on(switches_state, CONDITIONER);
        } else if (temp_inside <= 25) {
            turn_off(switches_state, CONDITIONER);
        }

        // Управление внутренним светом и цветовой температурой
        if (lights == "on") {
            turn_on(switches_state, LIGHTS_INSIDE);
            int color_temp = calculate_color_temperature(hour);
            std::cout << "Color temperature: " << color_temp << "K" << std::endl;
        } else {
            turn_off(switches_state, LIGHTS_INSIDE);
        }

        // Переход к следующему часу
        hour++;
        if (hour == 24) {
            hour = 0;
            day++;
        }
    }

    return 0;
}