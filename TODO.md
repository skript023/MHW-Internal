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
- [ ] **Monster Part Health & Severable Tail Bar**
  - Menampilkan HP per bagian tubuh monster (Head, Wings, Legs, Body, Tail).
  - Indikator sisa HP ekor yang bisa dipotong (*severable tail cut remaining*).
- [ ] **Monster Status Buildup (Ailments Tracking)**
  - Bar akumulasi status: Poison, Paralysis, Sleep, Blast, Stun/KO, Exhaust.
- [ ] **Party DPS / Damage Meter**
  - Tabel kontribusi damage tiap anggota tim (total damage, DPS, persentase damage hunt, cart count).
- [ ] **Dropped Materials & Shiny ESP**
  - Penanda visual 3D (*bounding box* / icon) untuk drop material monster di tanah (sangat berguna di Guiding Lands).
- [ ] **Hunter Buff & Mantle Durations**
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
