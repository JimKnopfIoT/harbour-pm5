<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1">
<context>
    <name>ActionTile</name>
    <message>
        <location filename="../qml/components/ActionTile.qml" line="30"/>
        <source>geplant</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DataField</name>
    <message>
        <location filename="../qml/components/DataField.qml" line="47"/>
        <source>tap to fold</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/DataField.qml" line="48"/>
        <source>%1 characters · tap to expand</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>EmulatePage</name>
    <message>
        <location filename="../qml/pages/EmulatePage.qml" line="29"/>
        <source>UID (hex, 4 / 7 / 10 bytes)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/EmulatePage.qml" line="42"/>
        <source>Start</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/EmulatePage.qml" line="48"/>
        <source>Stop</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/EmulatePage.qml" line="57"/>
        <source>What UID emulation does</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/EmulatePage.qml" line="59"/>
        <source>Presents the chosen UID as a 14443-A tag (DESFire for a 7-byte UID)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/EmulatePage.qml" line="60"/>
        <source>Enough for readers that only check the UID</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/EmulatePage.qml" line="63"/>
        <source>A reader with real DESFire crypto auth (e.g. your time system) is NOT fooled by the UID — that needs the card&apos;s key and full crypto emulation, which this app does not do.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/EmulatePage.qml" line="65"/>
        <source>Start, then hold the PM5 to the reader; press Stop to end.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ExportDialog</name>
    <message>
        <location filename="../qml/components/ExportDialog.qml" line="16"/>
        <source>Export</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/ExportDialog.qml" line="16"/>
        <source>Export log</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/ExportDialog.qml" line="21"/>
        <source>Format</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/ExportDialog.qml" line="33"/>
        <source>File name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/ExportDialog.qml" line="40"/>
        <source>Folder</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/ExportDialog.qml" line="53"/>
        <source>→ %1/%2.%3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/ExportDialog.qml" line="60"/>
        <source>Pick a folder</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Glossary</name>
    <message>
        <location filename="../qml/components/Glossary.qml" line="9"/>
        <source>Reference</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="15"/>
        <source>Anticollision: NFCID1 and SAK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="18"/>
        <source>What a card reveals before any application is selected — the layer the Proxmark reaches on every card.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="19"/>
        <source>SAK is the fastest tell: bit 0x20 means ISO 14443-4 and an ATS follows, 0x08/0x18 mean MIFARE Classic, 0x00 a Type 2 tag.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="21"/>
        <source>The anticollision identifier, 4/7/10 bytes (single/double/triple). A 4-byte value starting with 08 is not an identity — it is randomly generated per session, which payment cards, passports and identity cards all do on purpose.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="22"/>
        <source>The answer to the request command. Its top two bits encode the UID size; the low bits are the bit-frame anticollision setting.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="23"/>
        <source>One byte, the answer to being singled out. 0x20 = speaks ISO 14443-4, ATS follows; 0x40 = NFC-DEP; 0x04 = UID not complete, another cascade level follows.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="24"/>
        <source>Type 2 tag — MIFARE Ultralight, NTAG213/215/216.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="25"/>
        <source>MIFARE Classic 1K / Mini / 4K — Crypto1, reachable with the Proxmark (see the MIFARE page).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="26"/>
        <source>Plain ISO 14443-4: DESFire, bank cards, identity documents.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="27"/>
        <source>A Classic part plus an ISO 14443-4 part at once.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="34"/>
        <source>Reading the ATS</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="37"/>
        <source>The Answer To Select, with which an ISO 14443-4 card introduces itself after activation. It says how to talk to the card, not what is on it.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="38"/>
        <source>FSCI and FWI matter most: a small frame with a long waiting time is what makes probing slow.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="40"/>
        <source>Format byte. Low nibble = FSCI, the largest frame the card accepts (0→16 … 8→256 byte). Upper bits flag which of TA/TB/TC follow.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="41"/>
        <source>Transmission rates: which multiples of 106 kbit/s (212/424/848) the card supports, per direction.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="42"/>
        <source>Timing. Upper nibble FWI sets how long the card may take to answer (FWT ≈ 302 µs × 2^FWI); lower nibble SFGI the grace period after activation.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="43"/>
        <source>Protocol options: whether the card supports CID (card identifier) and NAD (node addressing).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="50"/>
        <source>EMV / payment applications</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="53"/>
        <source>A contactless bank card is an ISO 14443-4 smart card. The reader selects applications by identifier and reads their public directory.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="54"/>
        <source>Nothing secret is readable: the keys never leave the card&apos;s secure element, and every real transaction needs a fresh cryptogram over an unpredictable terminal number.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="56"/>
        <source>2PAY.SYS.DDF01 — the Proximity Payment System Environment. A directory listing the applications the card offers. German girocards often omit it, so known AIDs are tried directly.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="57"/>
        <source>Application Identifier, e.g. A0000000041010 = Mastercard, A0000000031010 = Visa, D276… = girocard.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="58"/>
        <source>A human-readable name the card gives its application, e.g. &quot;MASTERCARD&quot; or &quot;Debit&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="59"/>
        <source>File Control Information — the response to selecting an application, a BER-TLV structure holding the label, priorities and processing options.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="66"/>
        <source>Status words</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="69"/>
        <source>Every APDU answer ends in two bytes, SW1 SW2. A card that refuses is not a card that is absent.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="70"/>
        <source>6A82 means the application is genuinely not there. 6982 means it is there and will not talk — what a German eID says to any terminal without an authorisation certificate.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="72"/>
        <source>Success.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="73"/>
        <source>File or application not found. Treated as absent.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="74"/>
        <source>Security status not satisfied — present, but authentication is missing.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="75"/>
        <source>Conditions of use not satisfied — right command, wrong moment.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="76"/>
        <source>Verification failed; a PIN retry counter just went down.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="77"/>
        <source>Success, xx more bytes waiting — fetch with GET RESPONSE.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/Glossary.qml" line="78"/>
        <source>Wrong Le; repeat with xx as the expected length.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>HfInfoPage</name>
    <message>
        <location filename="../qml/pages/HfInfoPage.qml" line="26"/>
        <source>What the HF reader does</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/HfInfoPage.qml" line="28"/>
        <source>Activates any ISO 14443-A card: UID/NFCID1, ATQA and SAK, and the ATS of a 14443-4 card</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/HfInfoPage.qml" line="29"/>
        <source>Names the payment applications (PPSE, then known AIDs) and their labels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/HfInfoPage.qml" line="30"/>
        <source>On a payment card, reads the on-card transaction log when the card releases it</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/HfInfoPage.qml" line="31"/>
        <source>Tells MIFARE Classic apart from a smart card by its SAK — the MIFARE page then attacks it</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/HfInfoPage.qml" line="34"/>
        <source>Nothing secret. Keys stay in the card; a payment card cannot be cloned or replayed from a read.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/HfInfoPage.qml" line="36"/>
        <source>A UID starting with 08 is random and regenerated each session — it is not the card and buys nothing at a terminal.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LfInfoPage</name>
    <message>
        <location filename="../qml/pages/LfInfoPage.qml" line="25"/>
        <source>What the LF reader does</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/LfInfoPage.qml" line="27"/>
        <source>Reads EM4100, HID Prox and IO Prox 125 kHz badges</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/LfInfoPage.qml" line="28"/>
        <source>Clones an EM4100 id onto a blank T5577 — a real duplicate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/LfInfoPage.qml" line="31"/>
        <source>Encrypted LF (e.g. Hitag with keys) is not cloned; that is the Hitag page (planned).</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LfReference</name>
    <message>
        <location filename="../qml/components/LfReference.qml" line="7"/>
        <source>Reference</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/LfReference.qml" line="11"/>
        <source>125 kHz formats</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/LfReference.qml" line="14"/>
        <source>Low-frequency badges carry a fixed number and no cryptography — which is why they clone onto a blank so easily.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/LfReference.qml" line="16"/>
        <source>40-bit read-only id, modulated RF/64. The classic building/fob transponder. Clones onto a T5577.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/LfReference.qml" line="17"/>
        <source>HID 125 kHz Wiegand: a facility code plus a card number. Read-only in the tag.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/LfReference.qml" line="18"/>
        <source>Kantech ioProx, same idea as HID Prox with a different frame.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/LfReference.qml" line="19"/>
        <source>A programmable 125 kHz transponder that can emulate EM4100, HID and others. This is the blank the reader writes a cloned id onto.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/LfReference.qml" line="20"/>
        <source>The bit rate: one data bit per 64 carrier cycles. EM4100&apos;s standard clock.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LogPage</name>
    <message>
        <location filename="../qml/pages/LogPage.qml" line="26"/>
        <source>Export log</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/LogPage.qml" line="31"/>
        <source>Saved: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/LogPage.qml" line="31"/>
        <source>Export failed</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LogView</name>
    <message>
        <location filename="../qml/components/LogView.qml" line="26"/>
        <source>Log</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/LogView.qml" line="43"/>
        <source>No activity yet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/LogView.qml" line="50"/>
        <source>Export log</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/LogView.qml" line="56"/>
        <source>Saved: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/LogView.qml" line="56"/>
        <source>Export failed</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MainPage</name>
    <message>
        <location filename="../qml/pages/MainPage.qml" line="71"/>
        <source>Vicinity · SLIX</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/MainPage.qml" line="73"/>
        <source>Picopass</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/MainPage.qml" line="75"/>
        <source>Typ B</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/MainPage.qml" line="77"/>
        <source>NFC-F</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/MainPage.qml" line="79"/>
        <source>Prime</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/MainPage.qml" line="81"/>
        <source>LF · Hitag2/S</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/MainPage.qml" line="83"/>
        <source>LF · read/write</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/MainPage.qml" line="85"/>
        <source>LF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/MainPage.qml" line="87"/>
        <source>860–960 MHz · Roadmap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/MainPage.qml" line="89"/>
        <source>HF/LF Trace</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MifaInfoPage</name>
    <message>
        <location filename="../qml/pages/MifaInfoPage.qml" line="25"/>
        <source>What the MIFARE tools do</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/MifaInfoPage.qml" line="27"/>
        <source>Check every sector against a dictionary of default keys</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/MifaInfoPage.qml" line="28"/>
        <source>Dump all blocks with the found keys, rebuilding the trailers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/MifaInfoPage.qml" line="31"/>
        <source>A card with custom keys will not open — that needs nested/hardnested (planned).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/MifaInfoPage.qml" line="32"/>
        <source>Writing/cloning to a magic card is planned.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MifareReference</name>
    <message>
        <location filename="../qml/components/MifareReference.qml" line="7"/>
        <source>Reference</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/MifareReference.qml" line="11"/>
        <source>Sectors, blocks, keys</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/MifareReference.qml" line="14"/>
        <source>MIFARE Classic 1K is 16 sectors of 4 blocks (16 byte each). Each sector is locked by its own two keys.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/MifareReference.qml" line="15"/>
        <source>A sector opens only with the right key; the check tries a dictionary of well-known defaults.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/MifareReference.qml" line="17"/>
        <source>1K = 16 sectors × 4 blocks. Block 0 holds the UID; the last block of each sector is its trailer.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/MifareReference.qml" line="18"/>
        <source>The 4th block: Key A (6 byte) · access bits (4) · Key B (6). Key A never reads back — the dump rebuilds it from the found key.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/MifareReference.qml" line="19"/>
        <source>Two keys per sector. Access bits decide what each may do — often A reads, B reads and writes.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/MifareReference.qml" line="20"/>
        <source>MIFARE&apos;s proprietary 48-bit cipher. Broken since 2008; the phone&apos;s own NFC stack still refuses these cards, the Proxmark does not.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/MifareReference.qml" line="21"/>
        <source>Factory/common keys like FFFFFFFFFFFF or A0A1A2A3A4A5. Many cheap cards are never re-keyed and open straight away.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/MifareReference.qml" line="27"/>
        <source>If default keys fail</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/MifareReference.qml" line="30"/>
        <source>A card with custom keys will not open from the dictionary.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/MifareReference.qml" line="32"/>
        <source>Statistical attacks that recover an unknown key from the card&apos;s own weak randomness. The firmware collects the nonces; the key search is heavy host work (planned).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/MifareReference.qml" line="33"/>
        <source>A special blank whose block 0 / UID is writable — the target a dump is cloned onto (planned).</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PinDialog</name>
    <message>
        <location filename="../qml/components/PinDialog.qml" line="15"/>
        <source>Try PIN</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/PinDialog.qml" line="15"/>
        <source>Offline PIN</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/PinDialog.qml" line="23"/>
        <source>A wrong PIN spends one of the card&apos;s few tries. The app sends it only while at least 2 tries remain, so it can never block the card — but repeated wrong entries count the card down. Nothing is shown, logged or kept.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/PinDialog.qml" line="32"/>
        <source>PIN (half-blind)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/components/PinDialog.qml" line="33"/>
        <source>PIN</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Proxmark</name>
    <message>
        <location filename="../src/proxmark.cpp" line="30"/>
        <source>Not connected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="128"/>
        <source>open(%1) failed: %2 (errno %3)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="137"/>
        <source>tcgetattr failed: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="150"/>
        <source>tcsetattr failed: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="207"/>
        <source>write failed: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="292"/>
        <source>No %1 - is the Proxmark plugged in?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="302"/>
        <source>Port open, probing...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="305"/>
        <source>Port opened but the Proxmark did not answer a PING. It may run older firmware that only speaks the legacy frame format.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="309"/>
        <source>Proxmark answered. Reading version...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="322"/>
        <source>Disconnected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="342"/>
        <location filename="../src/proxmark.cpp" line="434"/>
        <source>PM5: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="422"/>
        <source>Connected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="437"/>
        <source>Connected (no version reply)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="605"/>
        <source>Checking sector %1/%2...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="610"/>
        <source>Sector %1  A:%2  B:%3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="621"/>
        <source>%1 of %2 sectors open with a default key</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="657"/>
        <source>Run the key check first</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="670"/>
        <source>Dumping sector %1/%2...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="700"/>
        <source>Dumped %1 of %2 blocks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="901"/>
        <source>selectable</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="902"/>
        <source>present · PACE/PIN required</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="905"/>
        <source>Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="970"/>
        <source>EMV application</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="972"/>
        <source>Payment card (EMV)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1073"/>
        <source>not tried — card hides its PIN-try counter, not risking a try</source>
        <translation type="unfinished"></translation>
    </message>
    <message numerus="yes">
        <location filename="../src/proxmark.cpp" line="1074"/>
        <source>not tried — only %n try(ies) left, would risk blocking</source>
        <translation type="unfinished">
            <numerusform></numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1098"/>
        <source>card refuses offline PIN here (SW %1) — no try spent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1215"/>
        <source>Transaction %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="997"/>
        <source>Transaction log</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="429"/>
        <source>ROM · %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="428"/>
        <source>Firmware · %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="427"/>
        <source>Device · Proxmark 5 @ %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="998"/>
        <source>present, protected — tap to try an offline PIN</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1072"/>
        <location filename="../src/proxmark.cpp" line="1101"/>
        <location filename="../src/proxmark.cpp" line="1105"/>
        <source>PIN</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1092"/>
        <source>wrong PIN · %1 tries left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1094"/>
        <source>PIN blocked / not usable</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1105"/>
        <source>verified ✓</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1224"/>
        <source>Trying offline PIN...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1230"/>
        <source>PIN attempt done</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1392"/>
        <source>UID must be 4, 7 or 10 bytes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1411"/>
        <source>Could not start emulation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1418"/>
        <source>Emulating %1 — hold the PM5 to the reader, then Stop</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1431"/>
        <source>Emulation stopped</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1499"/>
        <source>ISO 14443-B</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1501"/>
        <source>PUPI / NFCID0</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1502"/>
        <source>ATQB</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1503"/>
        <source>Application data</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1504"/>
        <source>Protocol info</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1505"/>
        <source>Card type</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1505"/>
        <source>German eID, passport, many girocard</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1507"/>
        <source>Type B · PUPI %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1509"/>
        <source>ISO 14443-B card · PUPI %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1523"/>
        <source>Reading 13.56 MHz - hold the card on the HF antenna...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1539"/>
        <source>Could not start HF read</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1560"/>
        <location filename="../src/proxmark.cpp" line="1567"/>
        <source>No Type A card - trying Type B...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1561"/>
        <source>No card in the HF field</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1568"/>
        <source>No 13.56 MHz card found</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1595"/>
        <source>UID / NFCID1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1597"/>
        <source>UID length</source>
        <translation type="unfinished"></translation>
    </message>
    <message numerus="yes">
        <location filename="../src/proxmark.cpp" line="1597"/>
        <source>%n byte(s)</source>
        <translation type="unfinished">
            <numerusform></numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1598"/>
        <source>ATQA</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1601"/>
        <source>SAK / SEL_RES</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1603"/>
        <source>  · Crypto1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1605"/>
        <source>ATS</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1606"/>
        <source>ATS decoded</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1609"/>
        <source>ATQA %1%2 · SAK %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1615"/>
        <source>ISO 14443-4 - probing applications...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1619"/>
        <source>Card class</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1620"/>
        <source>eID / passport / girocard (content needs PACE/keys)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1624"/>
        <source>%1 · UID %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1636"/>
        <source>Not a valid hex id: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1656"/>
        <source>Writing %1 to T5577 - hold the tag on the LF antenna...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1659"/>
        <source>Could not start the write</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1682"/>
        <source>Written: T5577 now answers as EM4100 %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1684"/>
        <source>Write rejected by the device (no tag, or not a T5577?)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1686"/>
        <source>No reply to the write - was a tag on the antenna?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1698"/>
        <source>Searching 125 kHz - hold the tag on the LF antenna...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1710"/>
        <source>Trying %1...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1722"/>
        <source>%1: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="1724"/>
        <source>No 125 kHz badge found (blank tag, or an unsupported format)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../src/proxmark.cpp" line="523"/>
        <source>MIFARE Ultralight / NTAG (Type 2)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="524"/>
        <source>MIFARE Classic 1K</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="525"/>
        <source>MIFARE Classic Mini</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="526"/>
        <source>MIFARE Classic 4K</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="527"/>
        <source>Infineon MIFARE Classic 1K</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="528"/>
        <source>MIFARE Classic + ISO-DEP</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="529"/>
        <source>MIFARE Plus 2K (SL2)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="530"/>
        <source>MIFARE Plus 4K (SL2)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="531"/>
        <source>ISO 14443-4 (DESFire, Plus SL3, JCOP…)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/proxmark.cpp" line="532"/>
        <source>unknown (SAK %1)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TerminalPage</name>
    <message>
        <location filename="../qml/pages/TerminalPage.qml" line="75"/>
        <source>Quick command</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/TerminalPage.qml" line="98"/>
        <source>Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/TerminalPage.qml" line="109"/>
        <source>Hold session</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/TerminalPage.qml" line="110"/>
        <source>Keep one activation across APDUs (SELECT → GPO → …)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/TerminalPage.qml" line="117"/>
        <source>APDU (hex)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/TerminalPage.qml" line="117"/>
        <source>cmd-id [payload] (hex)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/TerminalPage.qml" line="125"/>
        <source>Send</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/TerminalPage.qml" line="138"/>
        <source>Output</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/TerminalPage.qml" line="147"/>
        <source>Export log</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/TerminalPage.qml" line="152"/>
        <source>Saved: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/pages/TerminalPage.qml" line="152"/>
        <source>Export failed</source>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
