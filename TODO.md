# Daftar Fitur & Ide Pengembangan Monster Hunter: World (TODO)

Dokumen ini merangkum kumpulan fitur menarik dan populer dari komunitas modding, cheat tables (*MHW IB All-for-One Safi Full*, *Squall8*, *Cake-san*), serta overlay profesional (*HunterPie*, *SmartHunter*).

---

## 1. Visual & HUD Overlay (Inspirasi HunterPie & SmartHunter)

- [x] **Crown Size Indicator** *(Implemented)*
  - Indikator persentase ukuran monster dengan ikon **Miniature Crown 👑** ($\le 90\%$), **Silver Crown 🥈** ($\ge 115\%$), dan **Giant Gold Crown 👑** ($\ge 123\%$) langsung di health overlay.
- [x] **Monster Enrage / State Timer Bar** *(Implemented)*
  - Bar durasi **Enrage / Agitator** sekunder (warna oranye) dengan countdown detik dan persentase durasi aktif untuk kalkulasi Clutch Claw Flinch Shot.
- [x] **Monster 3D Coordinates & Range Alert** *(Implemented)*
  - Posisi koordinat 3D `[X, Y, Z]` dan jarak dinamis dengan indikator kedekatan tempur (`[NEAR]`, `[MID]`, `[FAR]`).
- [x] **Hunter Coordinates & World Phase Tracking** *(Implemented)*
  - Posisi koordinat hunter 3D, jarak ke Waypoint, cuaca dunia, dan siklus waktu (Day/Dusk/Night).
- [x] **Modular Overlay Settings** *(Implemented)*
  - Toggle individual di menu Settings Astra untuk mengaktifkan/menonaktifkan Monster Health Bar, Crown Badges, Enrage Bar, dan Hunter Info secara terpisah.
- [x] **Monster Part Health & Severable Tail Bar** *(Implemented)*
  - Menampilkan HP per bagian tubuh monster (Head, Wings, Legs, Body, Tail).
  - Indikator sisa HP ekor yang bisa dipotong (*severable tail cut remaining*).
- [x] **Monster Status Buildup (Ailments Tracking)** *(Implemented)*
  - Bar akumulasi status: Poison, Paralysis, Sleep, Blast, Stun/KO, Exhaust.
- [x] **Party DPS / Damage Meter** *(Implemented)*
  - Tabel kontribusi damage tiap anggota tim (total damage, DPS, persentase damage hunt, cart count).
- [x] **Dropped Materials & Shiny ESP** *(Implemented)*
  - Penanda visual 3D (*bounding box* / icon) untuk drop material monster di tanah (sangat berguna di Guiding Lands).
- [x] **Hunter Buff & Mantle Durations** *(Implemented)*
  - Timer bar untuk Demondrug, Might Seed, Demon Powder, Dash Juice, dan sisa durasi/cooldown Mantle yang sedang dipakai.

---

## 2. Mekanisme Senjata (Weapon Buffs & Mechanics)

- [ ] **Long Sword (LS)**
  - Infinite Spirit Gauge & Kunci Spirit Gauge di Red Level (Spirit 3).
  - Instant Spirit Helm Breaker / Fast Iai Spirit Slash.
- [ ] **Charge Blade (CB)**
  - Permanent Red Shield (Element Boost tak terbatas).
  - Permanent Red Sword (Mind's Eye + elemental phial boost).
  - Infinite Power Axe / Savage Axe Mode.
  - Instant Max Phials.
- [ ] **Switch Axe (SA)**
  - Permanent Amped State (Zero Sum Discharge selalu siap).
  - Infinite Sword Gauge (tidak pernah overheat / reload).
- [ ] **Insect Glaive (IG)**
  - Infinite Triple Extract Buff (Red + White + Orange selalu aktif).
  - Infinite Kinsect Stamina.
- [ ] **Dual Blades (DB)**
  - Infinite Demon Mode (tidak mengonsumsi stamina sama sekali).
  - Permanent Archdemon Mode Gauge.
- [ ] **Gunlance (GL)**
  - Infinite Wyrmstake Blast Duration (WSB nempel selamanya di monster).
  - Infinite Shells & No Wyvern's Fire Cooldown.
- [ ] **Hunting Horn (HH)**
  - Instant Melody Cast (semua buff lagu aktif otomatis tanpa memainkan notasi).
- [ ] **Bowgun (Heavy / Light Bowgun)**
  - No Recoil & No Reload (Bottomless Clip / infinite magazine).
  - Rapid-fire Wyvernheart (HBG) & Anti-Overheat.
  - Instant Wyvernsnipe Recharge.
- [ ] **Bow**
  - Instant Charge Level 4 (setiap tembakan langsung charge maksimal).
  - Infinite Coatings (Power Coating, Close Range, dll. tidak berkurang).
- [ ] **Hammer**
  - Permanent Power Charge (Blue Hammer buff permanen).

---

## 3. Combat & Monster Hunting QoL

- [ ] **Always Tenderized / 1-Hit Clutch Claw Wounding**
  - Setiap serangan Clutch Claw langsung melunakkan (*tenderize*) bagian monster tanpa delay, atau durasi tenderize permanen.
- [ ] **Super Armor / Rocksteady Mantle Effect**
  - Kebal terhadap monster roar, tremor, wind pressure, dan knockback tanpa perlu memakai mantel.
- [ ] **Infinite Mantle & Zero Cooldown**
  - Mantel (Temporal, Rocksteady, Ghillie, Evasion, dll.) tidak pernah habis durasinya dan langsung siap dipakai kembali setelah dilepas.
- [ ] **Infinite Mount & Instant Finisher**
  - Tidak pernah terjatuh saat menaiki monster, langsung dapat melakukan finisher attack.
- [ ] **Damage Multiplier Slider**
  - Slider pengali damage (1.0x hingga 10.0x) atau toggle One Hit Kill (OHK) untuk pengujian.
- [ ] **Instant Monster Part Breaker**
  - Pengali damage part break agar part monster lebih cepat patah / terpotong.

---

## 4. Guiding Lands & Mini-Games Automation

- [ ] **Guiding Lands Level Editor**
  - Mengubah level region Guiding Lands secara instan (Level 1 s/d Level 7) untuk Forest, Wildspire, Coral, Rotted, Volcanic, dan Tundra.
- [ ] **Infinite Guiding Lands Special Tracks / Lures**
  - Menambah lure monster apa saja ke daftar pancingan Guiding Lands dengan 1 klik.
- [ ] **Steamworks 100% Auto-Solver & Max Fuel**
  - Otomatis menebak tombol Steamworks dengan benar (100% Jackpot rate).
  - Pengali Steam Gauge & bahan bakar (Natural Fuel) tidak terbatas.
- [ ] **Infinite Lucky Vouchers**
  - Tiket Lucky Voucher selalu 99+ untuk quest rewards maksimal.

---

## 5. Eksplorasi, Pergerakan & Kamera

- [ ] **Freecam & Custom FOV Slider**
  - Mode kamera bebas (*fly cam*) untuk foto dan inspeksi monster.
  - Pengatur *Field of View* (FOV) horizontal/vertikal game di luar limit setting bawaan.
- [ ] **Movement Speed Multiplier & Noclip**
  - Mengatur kecepatan lari hunter di luar pertarungan (*speed boost*).
  - Fitur tembus tebing/dinding (*noclip*) untuk mencapai monster yang berada di tebing tinggi dengan cepat.
- [ ] **Infinite Gathering & Multi-Carve**
  - Ekstraksi tambang (*mining outcrops*) dan tulang (*bonepiles*) tanpa habis.
  - Carve tubuh monster berkali-kali tanpa batas waktu (Infinite Carving).
- [ ] **Rare Drop Modifier**
  - Memprioritaskan drop langka saat mengukir (Gems, Mantles, Immortal Reactor, Large Elder Dragon Gems).
- [ ] **All Monsters & Tracks Visible**
  - Menampilkan posisi semua monster dan jejak di peta minimap secara permanen tanpa perlu mengumpulkan Scoutfly.

---

## 6. Item, Peralatan & Dekorasi

- [ ] **Shop Item & Decoration Injector**
  - Membuka toko Provisions agar dapat membeli semua item konsumsi, material monster, dan permata langka (Attack Jewel 4, Expert Jewel 4) seharga 1 Zeni.
- [ ] **Safi'jiiva Awakened Ability Editor**
  - Mengubah kemampuan senjata Safi'jiiva (Attack VI, Sharpness V, Element, Slot, Essence) tanpa membuang Dracolite.
- [ ] **Augment Requirement Bypass**
  - Mengabaikan syarat material saat melakukan augmentasi senjata dan zirah di Smithy.
- [ ] **Layered Transmog Unlocker**
  - Membuka seluruh tampilan layered armor dan weapon tanpa harus menyelesaikan quest event atau grinding arena.

---

## 7. Quality of Life & Flow Permainan (Inspirasi Komunitas Nexus Mods)

- [x] **Skippable Cutscenes** *(Inspirasi: Skippable Cutscenes by Moonbunnie / AsteriskAmpersand)*
  - Melewati cutscene cerita (*story cutscenes*) yang terkunci dan tidak bisa di-skip hanya dengan menekan 1 tombol (Start / Esc), langsung melanjutkan gameplay tanpa menunggu.
- [x] **Post-Quest Wait Timer Skip** *(Inspirasi: FastResult / Standalone Quest End Timer Skip)*
  - Menghilangkan hitungan mundur tunggu 60 detik (hunt) atau 20 detik (capture) setelah monster mati, langsung menampilkan layar reward dan kembali ke hub/camp seketika.
- [x] **Instant Gathering & Zero-Delay Carving** *(Inspirasi: Fast Carve & Fast Gathering)*
  - Mengeliminasi animasi berlutut berulang kali saat mengukir monster (carving delay 0.1 detik tanpa interupsi monster kecil).
  - Ekstraksi tambang dan tulang sekali sentuh langsung mengumpulkan seluruh material node secara instan.
- [x] **Full Hub Sprint Speed** *(Inspirasi: Fast Seliana & Hub Movement)*
  - Hunter dapat berlari sprint dengan kecepatan maksimal di dalam markas (Astera, Seliana, Gathering Hubs) tanpa terhambat animasi jalan lambat di tangga atau tanjakan.
- [x] **Remote Tent & Item Box Access Anywhere** *(Inspirasi: Portable Camp Facilities)*
  - Membuka menu ganti perlengkapan (*Change Equipment*) dan kotak item (*Item Box*) dari mana saja di map/expedition tanpa perlu kembali ke base camp.
- [x] **Quick Quest Restart & Reset** *(Inspirasi: Quick Quest Restart)*
  - Opsi tombol 1 klik untuk langsung me-restart quest aktif dari awal (sangat berguna untuk speedrunning dan arena time-attack).

---

## 8. Sieges, End-Game & Grinding Reducers (Inspirasi Nexus Mods & ICE)

- [ ] **Solo Siege Scaling (Safi'jiiva & Kulve Taroth)** *(Inspirasi: Solo Safi / Kulve Scaling)*
  - Mengubah kalkulasi drain energi lantai dan total HP Safi'jiiva & Kulve Taroth ke skala 1 pemain (*True Solo Scaling*), memungkinkan menyelesaikan siege raid secara mandiri dalam 1 run tanpa grinding berjam-jam di lobby multiplayer.
- [ ] **Guiding Lands Level Cap Remover** *(Inspirasi: Kat's Guiding Lands Cap Remover)*
  - Menghapus batas total akumulasi level region Guiding Lands sehingga seluruh 6 region (Forest, Wildspire, Coral, Rotted, Volcanic, Tundra) dapat berada di Level 7 secara bersamaan (*anti de-leveling*).
- [ ] **Permanent Geologist Exploit (2x/3x Shinies Gather)** *(Inspirasi: Geology QoL)*
  - Mengunci efek pengumpulan ganda pada drop material monster di tanah (khususnya Guiding Lands) agar dapat dipungut 2–3 kali per drop tanpa syarat memakai skill Geologist level 1.
- [ ] **Botanical Research & Soft Soil Auto-Maintainer** *(Inspirasi: Fertilizer Counter & Max Farm)*
  - Mengunci durasi pupuk / *Soft Soil* pada level dan durasi maksimal (99 turn) secara permanen sehingga tanaman kebun selalu panen maksimal tanpa perlu bolak-balik ke NPC Botanical.
- [ ] **Decoration Drop RNG Overhaul / Smart Feystone Reroll** *(Inspirasi: Decoration Drop Rate Overhaul)*
  - Menyesuaikan bobot probabilitas *Sealed Feystone* dan *Carved Feystone* agar memprioritaskan dekorasi langka tier-12 (Attack Jewel 4, Expert Jewel 4, Challenger Jewel 4, Agitator 4) daripada dekorasi sampah.

---

## 9. Clutch Claw & Sistem Tempur Lanjutan (Inspirasi Nexus Mods & ICE)

- [ ] **Infinite Clutch Claw Reach** *(Inspirasi: Infinite / Longer Reach Clutch Claw)*
  - Menambah jangkauan kait Clutch Claw (slider jangkauan 2x hingga tak terbatas), mempermudah menempel ke monster terbang atau monster berukuran raksasa.
- [ ] **Universal 1-Hit Tenderize & Guaranteed Slinger Ammo** *(Inspirasi: ICE Clutch Claw Rework)*
  - Serangan cakar senjata ringan (LS, DB, Bow, SnS, IG, LBG) langsung melunakkan (*tenderize*) bagian monster dalam 1 kali serangan DAN tetap menjatuhkan batu/pod slinger ammo.
- [ ] **Universal Wall-Bang / Unrestricted Flinch Shot** *(Inspirasi: Unrestricted Flinch Shot)*
  - Menembakkan slinger ammo ke monster saat menempel di kepala akan selalu memicu dorongan ke dinding (*wall-bang*) bahkan saat monster sedang dalam kondisi Enrage (mata kuning/merah) atau dari sudut mana pun.
- [ ] **Extended Tenderize Duration Slider** *(Inspirasi: Longer Tenderize)*
  - Mengatur durasi kelembutan daging monster hasil tenderize (dari standar 180 detik menjadi 600 detik atau permanen).
- [ ] **Universal CC & Roar Immunity (True Rocksteady)** *(Inspirasi: QoL Immunity)*
  - Kebal alami terhadap Monster Roar (Earplugs 5), Wind Pressure (Windproof 5), Tremor (Tremor Resistance 3), dan Stun tanpa memerlukan mantel Rocksteady aktif atau membuang slot dekorasi.

---

## 10. Visual Hitzone & Analitik Tempur (Inspirasi Nexus Mods & SmartHunter)

- [x] **Monster Elemental & Status Weakness Icon HUD** *(Inspirasi: Monster Weakness Icon Indicator)*
  - Menampilkan panel ikon kelemahan elemen monster (Fire, Water, Thunder, Ice, Dragon) dan efektivitas status (Poison, Sleep, Paralysis, Blast, Stun) langsung di sebelah nama monster pada overlay.
- [x] **Hitzone Value (HZV) Vulnerability Indicator** *(Inspirasi: ICE HZV & SmartHunter)*
  - Menampilkan persentase ketahanan/kerentanan bagian tubuh monster terhadap tipe serangan senjata hunter (Severing / Slash, Blunt, Shot, dan Elemental HZV).
- [x] **Light Pillars for Dropped Items & Shinies** *(Inspirasi: Souvenir's Light Pillar & Dropped Items EFX)*
  - Memproyeksikan pilar cahaya vertikal 3D (*light beam*) di atas setiap drop material monster di tanah agar mudah terlihat di rumput tebal atau area gelap.
- [x] **Tic Rate & High-FPS Damage Calculation Fix** *(Inspirasi: Tic Rate Fix)*
  - Memperbaiki kalkulasi hit rate game pada framerate monitor tinggi (>60 FPS, 144Hz, 240Hz) agar tick damage serangan bertubi-tubi (seperti Piercing Shots, DB Blade Dance, Dragon Piercer) terhitung akurat sesuai frame asli.

---

## 11. Multiplayer Co-op & Team Utilities (Inspirasi HunterPie & SmartHunter)

- [x] **Co-op Team Player ESP & Health Bars** *(Implemented)*
  - Menampilkan radar 3D world space, floating nameplate, tipe senjata, sisa HP, dan jarak hunter anggota tim (P1 s/d P4) di layar dalam estetika MHW HUD.
- [x] **Off-Screen Teammate Compass Indicator** *(Implemented)*
  - Menampilkan panah kompas / chevron penunjuk arah di pinggiran layar saat rekan setim berada di belakang hunter atau di luar jarak pandang kamera, dilengkapi jarak meter.
- [x] **Instant Fast Travel / Teleport to Teammate** *(Implemented)*
  - Tombol 1 klik di menu Teleport serta pintasan cepat (Hotkey `F9` / klik nameplate) untuk langsung berteleportasi seketika ke samping rekan setim saat co-op di map luas (Ancient Forest, Guiding Lands, Coral Highlands).


