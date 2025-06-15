#pragma once

struct PlayerDataZ {
    std::string Name;
    std::string UserID;
    std::string Verified;
    std::string Rank;
    std::string Star;
    std::string grade;
    int uiRank;
    int HeroID;
    int Spell;
    std::string Country;
    std::string Version;
    std::string Gender;
    std::string Elo;
    std::string MPL;
    std::string Ip;
    std::string RoleLevel;
    std::string BattleCount;
};

struct {
    PlayerDataZ PlayerB[5]; //10
    PlayerDataZ PlayerR[5];
} RoomInfoZ;

void RoomInfoList() {
    void *LogicBattleManager_Instance;
    Il2CppGetStaticFieldValue("Assembly-CSharp.dll", "", "LogicBattleManager", "Instance", &LogicBattleManager_Instance);
    if (LogicBattleManager_Instance) {
        int iBattleState = GetBattleState();
        if (iBattleState == 2 || iBattleState == 3) {
            List<uintptr_t> *m_RoomPlayerInfo = GetBattlePlayerInfo();
            if (!m_RoomPlayerInfo) return;
            int PlayerB = 0, PlayerR = 0, iSelfCamp = -1;
            int m_uiID = *(int *)(SystemData_m_uiID);
            for (int i = 0; i < m_RoomPlayerInfo->getSize(); i++) {
                auto roomData = m_RoomPlayerInfo->getItems()[i];
                if (!roomData) continue;
                auto iCamp = *(int *) ((uintptr_t)roomData + RoomData_iCamp);
                auto lUid = *(int *) ((uintptr_t)roomData + RoomData_lUid);
                if (lUid != m_uiID) continue;
                iSelfCamp = iCamp;
                break;
            }
            for (int i = 0; i < m_RoomPlayerInfo->getSize(); i++) {
                auto roomData = m_RoomPlayerInfo->getItems()[i];
                if (!roomData) continue;
                auto iCamp = *(int *) ((uintptr_t)roomData + RoomData_iCamp);
                auto lUid = *(int *) ((uintptr_t)roomData + RoomData_lUid);
                auto uiZoneId = *(int *) ((uintptr_t)roomData + RoomData_uiZoneId);
                auto _sName = *(String **) ((uintptr_t)roomData + RoomData_sName);
                    auto _steamSimpleName = *(String **)((uintptr_t)roomData + RoomData__steamSimpleName);
                    auto _steamName = *(String **)((uintptr_t)roomData + RoomData__steamName);
                    auto uiRankLevel = *(int *) ((uintptr_t)roomData + RoomData_uiRankLevel);
                    auto iMythPoint = *(int *) ((uintptr_t)roomData + RoomData_iMythPoint);
                    auto heroid = *(int *) ((uintptr_t)roomData + RoomData_heroid);
                    auto summonSkillId = *(int *) ((uintptr_t)roomData + RoomData_summonSkillId);
                    
                if (iCamp == iSelfCamp) {
                    if (_sName) {
                        RoomInfoZ.PlayerB[PlayerB].Name = _sName->toString();
                    }
                    RoomInfoZ.PlayerB[PlayerB].UserID = to_string(lUid) + " (" + to_string(uiZoneId) + ")";
                    RoomInfoZ.PlayerB[PlayerB].Rank = RankToString(uiRankLevel, iMythPoint);
                    RoomInfoZ.PlayerB[PlayerB].Hero = HeroToString(heroid);
                    RoomInfoZ.PlayerB[PlayerB].Spell = SpellToString(summonSkillId);
                    PlayerB++;
                } else {
                    if (_sName) {
                        RoomInfoZ.PlayerR[PlayerR].Name = _sName->toString();
                    }
                    RoomInfoZ.PlayerR[PlayerR].UserID = to_string(lUid) + " (" + to_string(uiZoneId) + ")";
                    RoomInfoZ.PlayerR[PlayerR].Rank = RankToString(uiRankLevel, iMythPoint);
                    RoomInfoZ.PlayerR[PlayerR].Hero = HeroToString(heroid);
                    RoomInfoZ.PlayerR[PlayerR].Spell = SpellToString(summonSkillId);
                    PlayerR++;
                    }
                }
            }
        }
  }  

