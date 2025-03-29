#include <zmk/display/widgets.h>
#include <zmk/sensors/battery.h>
#include <zmk/events/layer_state_changed.h>

// Logo NOCLTT
static const char *NOCLTT_LOGO[] = {
    "███╗   ██╗ ██████╗██╗  ████████╗████████╗",
    "████╗  ██║██╔════╝██║  ╚══██╔══╝╚══██╔══╝",
    "██╔██╗ ██║██║     ██║     ██║      ██║   ",
    "██║╚██╗██║██║     ██║     ██║      ██║   ",
    "██║ ╚████║╚██████╗███████╗██║      ██║   ",
    "╚═╝  ╚═══╝ ╚═════╝╚══════╝╚═╝      ╚═╝   "
};

// Função para obter nome da layer atual
static const char* get_layer_name(uint8_t layer) {
    switch (layer) {
        case 0: return "BASE";
        case 1: return "LOWER";
        case 2: return "RAISE";
        case 3: return "FUNC";
        case 4: return "NUM";
        default: return "UNKNOWN";
    }
}

// Função de renderização do display
void custom_draw_display(lv_obj_t *canvas) {
    // Limpa o canvas
    lv_obj_clean(canvas);
    
    // Obtém informações do sistema
    uint8_t current_layer = zmk_get_highest_layer();
    int battery_level = zmk_battery_state_of_charge();
    
    // Obtém hora e data atuais
    struct tm time_info;
    zmk_get_time(&time_info);
    
    char time_str[9];
    char date_str[11];
    strftime(time_str, sizeof(time_str), "%H:%M:%S", &time_info);
    strftime(date_str, sizeof(date_str), "%d/%m/%Y", &time_info);
    
    // Coluna Esquerda
    char left_col[100];
    snprintf(left_col, sizeof(left_col), 
        "┌─[ TIME ]─\n"
        "│ %s\n"
        "├─[ BATT ]─\n"
        "│ %3d%%\n"
        "├─[ LAYER ]─\n"
        "│ %s", 
        time_str, 
        battery_level, 
        get_layer_name(current_layer)
    );
    
    // Coluna Direita
    char right_col[200];
    snprintf(right_col, sizeof(right_col), 
        "┌─[ DATE ]─\n"
        "│ %s\n"
        "├─[ LAYER ]─\n"
        "│ %s\n"
        "├─[ LOGO ]─\n"
        "│ NOCLTT", 
        date_str, 
        get_layer_name(current_layer)
    );
    
    // Renderiza colunas
    lv_canvas_draw_text(canvas, 0, 0, left_col, &lv_font_unscii_8, LV_COLOR_WHITE);
    lv_canvas_draw_text(canvas, 64, 0, right_col, &lv_font_unscii_8, LV_COLOR_WHITE);
    
    // Opcional: renderizar logo detalhado
    for (int i = 0; i < 6; i++) {
        lv_canvas_draw_text(canvas, 70, 40 + i*8, NOCLTT_LOGO[i], 
                             &lv_font_unscii_8, LV_COLOR_WHITE);
    }
}

// Função de inicialização do display personalizado
void custom_display_init(lv_obj_t *canvas) {
    // Configurações iniciais, se necessário
    lv_timer_create(custom_draw_display, 1000, canvas);  // Atualiza a cada segundo
}
