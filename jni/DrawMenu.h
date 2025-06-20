
bool showMenu = true;
bool bFullChecked = false;
int selectedFeatures = 1;
android_app *i_App = 0;
static bool isRoomInfo = false;

unsigned int gpCrash = 0xfa91b9cd;
static int crash(int randomval){
    volatile int *p = (int *)gpCrash;
    p += randomval;
    p += *p + randomval;
    p = 0;
    p += *p;
    return *p;
}

void CenteredText(ImColor color, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    const char *text, *text_end;
    ImFormatStringToTempBufferV(&text, &text_end, fmt, args);
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text, text_end).x) * 0.5);
    ImGui::TextColoredV(color, fmt, args);
    va_end(args);
}

inline ImVec4 RGBA2ImVec4(int r, int g, int b, int a) {
    float newr = (float)r / 255.0f;
    float newg = (float)g / 255.0f;
    float newb = (float)b / 255.0f;
    float newa = (float)a / 255.0f;
    return ImVec4(newr, newg, newb, newa);
}

void HideMenu(bool& bShow) {
    if (bShow) {
        ImGui::OpenPopup("ConfirmHide");
    }

    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("ConfirmHide", 0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse))
    {
        ImGui::Text("Are you sure you want to hide the menu?");
        if (ImGui::Button("Yes", ImVec2(ImGui::GetContentRegionAvail().x / 2, 0)))
        {
            showMenu = false;
            bShow = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("No", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
        {
            bShow = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

struct sTheme {
    bool TemaAsik;
    bool Color;
};
sTheme Theme{0};

std::string msg;

bool selectedThemes;

inline ImColor main_color(230, 134, 224, 255);

inline ImColor text_color[3] = {ImColor(255, 255, 255, 255), ImColor(200, 200, 200, 255), ImColor(150, 150, 150, 255) };

inline ImColor foreground_color(1.f, 1.f, 1.f, 0.15f);

inline ImColor background_color(13, 14, 16, 200);

inline ImColor second_color(255, 255, 255, 20);

inline ImColor stroke_color(35, 35, 35, 255);

inline ImColor child_color(19, 19, 19, 255);

inline ImColor scroll_bg_col(24, 24, 24, 255);

inline ImColor winbg_color(15, 16, 18, 200);

inline ImFont* icon_font;

inline ImFont* logo_font;

inline ImFont* icon_big_font;

inline ImFont* medium_font;

inline ImFont* small_font;

inline ImFont* small_icon_font;

inline ImFont* arrow_icons;

inline float anim_speed = 8.f;

static inline ImDrawList* foremenu_drawlist;

inline ImColor GetColorWithAlpha(ImColor color, float alpha)
{
return ImColor(color.Value.x, color.Value.y, color.Value.z, alpha);
}

inline ImVec2 center_text(ImVec2 min, ImVec2 max, const char* text)
{
    return min + (max - min) / 2 - ImGui::CalcTextSize(text) / 2;
}

inline int rotation_start_index;
inline void ImRotateStart()
{
    rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
}

inline ImVec2 ImRotationCenter()
{
    ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX); // bounds

    const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = rotation_start_index; i < buf.Size; i++)
        l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

    return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2); // or use _ClipRectStack?
}

inline void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter())
{
    float s = sin(rad), c = cos(rad);
    center = ImRotate(center, s, c) - center;

    auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = rotation_start_index; i < buf.Size; i++)
        buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
}

void Trinage_background()
{

    ImVec2 screen_size = ImVec2(glWidth, glHeight);

    static ImVec2 partile_pos[100];
    static ImVec2 partile_target_pos[100];
    static float partile_speed[100];
    static float partile_size[100];
    static float partile_radius[100];
    static float partile_rotate[100];

    for (int i = 1; i < 50; i++)
    {
        if (partile_pos[i].x == 0 || partile_pos[i].y == 0)
        {
            partile_pos[i].x = rand() % (int)screen_size.x + 1;
            partile_pos[i].y = screen_size.y + 30;
            partile_speed[i] = 70 + rand() % 135;
            partile_radius[i] = 2 + rand() % 10;
            partile_size[i] = 15 + rand() % 40;

            partile_target_pos[i].x = rand() % (int)screen_size.x;
            partile_target_pos[i].y = -50;
        }

        partile_pos[i].y -= ImGui::GetIO().DeltaTime * partile_speed[i];
        partile_rotate[i] -= ImGui::GetIO().DeltaTime;

        if (partile_pos[i].y < 10)
        {
            partile_pos[i].x = 0;
            partile_pos[i].y = 0;
            partile_rotate[i] = 0;
        }

        ImRotateStart();
        ImGui::GetWindowDrawList()->AddRectFilled(partile_pos[i] - ImVec2(partile_size[i], partile_size[i]), partile_pos[i] + ImVec2(partile_size[i], partile_size[i]), GetColorWithAlpha(main_color, 0.3f), 5.f);
        ImRotateEnd(partile_rotate[i]);
    }
}
int selectedOption = 0;

void DrawMenu() {
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(650, 680), ImGuiCond_FirstUseEver);

    ImGuiIO& io = ImGui::GetIO();

    static float window_scale;
    if (!window_scale) window_scale = 1.0f;
    io.FontGlobalScale = window_scale;

    static bool isLogin = false;
    static char s[64] = "";

    if (!ImGui::Begin("MLBB Mod Menu", 0, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::End();
        return;
    }

    static bool isPopUpHide = false;
    HideMenu(isPopUpHide);
    
    static bool bFlagAutoResize = true;
    static ImGuiWindowFlags window_flags;
    if (bFlagAutoResize) {
        window_flags = ImGuiWindowFlags_AlwaysAutoResize;
    } else {
        window_flags = ImGuiWindowFlags_None;
    }
	
    std::string FULLTITLE = std::string("XCODE MOD MENU");
    if (!ImGui::Begin(FULLTITLE.c_str(), 0, window_flags)) {
        ImGui::End();
        return;
    }
	
    using namespace ImGui;
	ImGui::SetNextWindowSize(ImVec2((float) glWidth * 0.3f, (float) glHeight * 0.5f),ImGuiCond_Once); // 45% width 70% height
	std::string devv = std::string("https://facebook.com/dafidxcode");

	// revjump bypass !isLogin to isLogin visual hack
    if (!isLogin) {
        if (ImGui::BeginTabBar("TabLogin", ImGuiTabBarFlags_FittingPolicyScroll)) {
            if (ImGui::BeginTabItem("Login Menu")) {
                ImGui::Text("Enter key to login:");
                ImGui::InputText("##key", s, sizeof s);

                if (ImGui::Button("Login", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
                    bool LoginOK = false;
                    msg = Login(g_vm, s, &LoginOK);
                    if (LoginOK) {
                    	isLogin = true;
                        loadBattleData(battleData);
                        bFullChecked = true;
			//UnlockSkin();
                    }
                }
		if (ImGui::Button("Paste Key", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
                        auto key = getClipboardText(g_vm);
                        strncpy(s, key.c_str(), sizeof s);
		}
        ImGui::Spacing();
		ImGui::Spacing();
		if (ImGui::Button("Get a Key", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
			openURL(g_vm, devv);
		}

        ImGui::Spacing();
        ImGui::TextColored(ImColor(255, 255, 0), "%s", msg.c_str());

            ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    } else { 
		if (ImGui::BeginTabBar("Tab", ImGuiTabBarFlags_FittingPolicyScroll)) {
		 
                 			
			if (selectedFeatures == 1 | selectedFeatures == 2){
				if (ImGui::BeginTabItem("ESP Player")) {
            	if (ImGui::CollapsingHeader("Player")) {
                	if (ImGui::BeginTable("ESPPlayer", 3)) {
                    	ImGui::TableNextColumn();	(" Player Line", &Config.ESP.Player.Line);
                        ImGui::TableNextColumn();	(" Player Box", &Config.ESP.Player.Box);
                        ImGui::TableNextColumn();	(" Player Name", &Config.ESP.Player.Name);
                        ImGui::TableNextColumn();	(" Player Hero", &Config.ESP.Player.Hero);
                        ImGui::TableNextColumn();	(" Player Health", &Config.ESP.Player.Health);
						ImGui::TableNextColumn();	(" Player Distance", &Config.ESP.Player.Distance);
						ImGui::TableNextColumn();	(" Player Locator", &Config.ESP.Player.Locator2);
                        ImGui::TableNextColumn();	(" Hero Alert", &Config.ESP.Player.Alert);
                        ImGui::TableNextColumn();	(" Icon Hero", &Config.ESP.Player.HeroZ);
                        ImGui::TableNextColumn();	(" Visible Check", &Config.ESP.Player.Visible);
                        ImGui::TableNextColumn();	(" FPS v1", &Config.ESP.Player.Fps30);
                        ImGui::TableNextColumn();	(" FPS v2", &Config.ESP.Player.Fps60);
                        ImGui::EndTable();
                    }
                }
                if (ImGui::CollapsingHeader("Monster")) {
                	if (ImGui::BeginTable("Monster", 2)) {
                    	ImGui::TableNextColumn();	ImGui::Checkbox(" Monster Round", &Config.ESP.Monster.Rounded);
                        ImGui::TableNextColumn();	ImGui::Checkbox(" Monster Health", &Config.ESP.Monster.Health);
                        ImGui::TableNextColumn();	ImGui::Checkbox(" Monster Name", &Config.ESP.Monster.Name);
                        ImGui::TableNextColumn();	ImGui::Checkbox(" Monster Alert", &Config.ESP.Monster.Alert);
						ImGui::TableNextColumn();	ImGui::Checkbox(" Monster Icon", &Config.ESP.Monster.Locator);
						ImGui::TableNextColumn();	ImGui::Checkbox(" Monster Locator", &Config.ESP.Monster.Locator2);
						ImGui::TableNextColumn();	ImGui::Checkbox(" Monster UID", &Config.m_IDConf);
						ImGui::TableNextColumn();	ImGui::Checkbox(" Minion Locator", &Config.m_IDConf);
                        ImGui::EndTable();
                    }
                }
                ImGui::EndTabItem();
			}
			}
			
		if (selectedFeatures == 1 | selectedFeatures == 2){
		if (ImGui::BeginTabItem("Additional")) {
                ImGui::Checkbox("Unlock All Skins", &Config.Visual.UnlockSkin); 
                ImGui::EndTabItem();
                 }
	        }
			
			if (selectedFeatures == 1 | selectedFeatures == 2){
			if (ImGui::BeginTabItem("Auto Aim")) { // Tab Auto Aim
                // Pindahkan konten Auto Aim ke dalam tab ini
                ImGui::BeginGroupPanel("Auto Aim", ImVec2(0.0f, 0.0f));
                ImGui::Spacing();
                ImGui::Checkbox("Auto Aim Basic", &Aim.Helper.Skills.Basic);
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Checkbox("Auto Aim Skil 1", &Aim.Helper.Skills.Skill1);
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Checkbox("Auto Aim Skil 2", &Aim.Helper.Skills.Skill2);
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Checkbox("Auto Aim Skil 3", &Aim.Helper.Skills.Skill3);
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Checkbox("Auto Aim Skil 4", &Aim.Helper.Skills.Skill4);
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::EndGroupPanel();
                ImGui::SameLine();
                ImGui::BeginGroupPanel("Priority", ImVec2(0.0f, 0.0f));
                ImGui::Text("Target Priority:");
                ImGui::RadioButton("Closest Distance", &Aim.Helper.Priority.Target, 0);
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::RadioButton("Lowest HP", &Aim.Helper.Priority.Target, 1);
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Text("Range Auto Aim:");
                ImGui::SliderFloat("##RangeFOV", &RangeFOV, 0, 200, "%.1fm");
                ImGui::Spacing();
                ImGui::EndGroupPanel();

                ImGui::Spacing();
                ImGui::EndTabItem(); // Akhir Tab Auto Aim
            }
            }
			if (selectedFeatures == 1 | selectedFeatures == 2){
				if (ImGui::BeginTabItem("Maphack")) {
					ImGui::BeginGroupPanel("Drone View", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f));
                        {
                            ImGui::PushItemWidth(-1);
                            ImGui::SliderFloat("##Drone View", &SetFieldOfView, 0, 100, "%.1f");
                            ImGui::PopItemWidth();
                            ImGui::Spacing();
                        }
                        ImGui::EndGroupPanel();
                ImGui::Checkbox("Minimap Icon", &Config.MinimapIcon);
                if (!Config.MinimapIcon) ImGui::BeginDisabled();
                ImGui::SameLine();
                ImGui::Checkbox("Hide Line", &Config.HideLine);
                ImGui::BeginGroup();
                {
                    ImGui::BeginGroupPanel("MiniMap Adjustable", ImVec2(-1.0f, 0.0f));
                   {
                        ImGui::BeginGroupPanel("Map Position", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f));
                        {
                            ImGui::PushItemWidth(-1);
                            ImGui::SliderFloat("##MapPosition", &StartPos.x, 0.0f, (float)(screenWidth / 2));
                            ImGui::PopItemWidth();
                            ImGui::Spacing();
                        }
                        ImGui::EndGroupPanel();

                        ImGui::BeginGroupPanel("Map Size", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f));
                        {
                            ImGui::PushItemWidth(-1);
                            ImGui::SliderInt("##MapSize", &MapSize, 0, 800);
                            ImGui::PopItemWidth();
                            ImGui::Spacing();
                        }
                        ImGui::EndGroupPanel();

                        ImGui::BeginGroupPanel("Icon Size", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f));
                        {
                            ImGui::PushItemWidth(-1);
                            ImGui::SliderInt("##IconSize", &ICSize, 0, 100);
                            ImGui::PopItemWidth();
                            ImGui::Spacing();
                        }
                        ImGui::EndGroupPanel();

                        ImGui::BeginGroupPanel("Health Thin", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f));
                        {
                            ImGui::PushItemWidth(-1);
                            ImGui::SliderInt("##HealthThin", &ICHealthThin, 0, 10);
                            ImGui::PopItemWidth();
                            ImGui::Spacing();
                        }
                        ImGui::EndGroupPanel();
						
						ImGui::BeginGroupPanel("Health Color", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f));
                        {
                            ImGui::PushItemWidth(-1);
                            ImGui::ColorEdit3("##HealthColor", ColorHealth);
                            ImGui::PopItemWidth();
                            ImGui::Spacing();
                        }
                        ImGui::EndGroupPanel();
														     
                        ImGui::Spacing();
                    }
                    ImGui::EndGroupPanel();
                    if (!Config.MinimapIcon) ImGui::EndDisabled();
                }
                ImGui::EndGroup();
                ImGui::EndTabItem();
            }
			}
			
			static int SelectInfo = 0;
            static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV;
            if (ImGui::BeginTabItem("Setting")) {
                ImGui::BeginGroupPanel("Menu Setting", ImVec2(-1.0f, 0.0f));
                {
                    ImGui::Checkbox("Auto Resize", &bFlagAutoResize);
                    ImGui::BeginGroupPanel("Window Size", ImVec2(-1.0f, 0.0f));
                    {
                        ImGui::PushItemWidth(-1);
                        ImGui::SliderFloat("##Scale", &window_scale, 0.5f, 2.5f, "%.1f");
                        ImGui::PopItemWidth();
                        ImGui::Spacing();
                    }
                    ImGui::EndGroupPanel();
                    
                    if (ImGui::Button("Hide Menu", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
                        isPopUpHide = true;
                    }

                    ImGui::BeginGroupPanel("English", ImVec2(-1.0f, 0.0f));
                    {
                        ImGui::TextColored(RGBA2ImVec4(255, 255, 0, 255), "To display the menu again,");
                        ImGui::TextColored(RGBA2ImVec4(255, 255, 0, 255), "simply touch on the lower left corner of your screen.");
                        ImGui::Spacing();
                    }
                    ImGui::EndGroupPanel();

                    ImGui::BeginGroupPanel("Indonesia", ImVec2(-1.0f, 0.0f));
                    {
                        ImGui::TextColored(RGBA2ImVec4(255, 255, 0, 255), "Untuk menampilkan kembali menu,");
                        ImGui::TextColored(RGBA2ImVec4(255, 255, 0, 255), "cukup sentuh di pojok kiri bawah layar Anda.");
                        ImGui::Spacing();
                    }
                    ImGui::EndGroupPanel();
                    
                    ImGui::Spacing();
                    
                    // if (ImGui::Button("Save Cheat Setting", ImVec2(ImGui::GetContentRegionAvail().x / 2, 0))) {
                    //     saveCFG();
                    // }
                    // ImGui::SameLine();
                    // if (ImGui::Button("Load Cheat Setting", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
                    //     loadCFG();
                    // }
					
        			ImGui::Separator();
					
					ImGui::Spacing();
                    ImGui::BeginGroupPanel("Mood Menu Info", ImVec2(-1.0f, 0.0f));
                    {
						Text("Games : "); SameLine();
                        TextColored(RGBA2ImVec4(176, 40, 40, 255), "Mobile Legends Bang Bang");
                        Text("Mod Status : "); SameLine();
                        ImGui::TextColored(RGBA2ImVec4(176, 40, 40, 255), "Free Version");

			       Text("Developer : "); SameLine();
                        Text("@Dafidxcode");
                    }
                    ImGui::EndGroupPanel();
                }
                ImGui::EndGroupPanel();
                ImGui::EndTabItem();
            }
			
			ImGui::EndTabBar();
		}
		ImGui::Separator();
        ImGui::TreePop();
	}
}
