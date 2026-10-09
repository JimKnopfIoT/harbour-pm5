<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="de">
<context>
    <name>ActionTile</name>
    <message>
        <source>geplant</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DataField</name>
    <message>
        <source>tap to fold</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 characters · tap to expand</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>EmulatePage</name>
    <message>
        <source>UID (hex, 4 / 7 / 10 bytes)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Start</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Stop</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>What UID emulation does</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Presents the chosen UID as a 14443-A tag (DESFire for a 7-byte UID)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enough for readers that only check the UID</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A reader with real DESFire crypto auth (e.g. your time system) is NOT fooled by the UID — that needs the card&apos;s key and full crypto emulation, which this app does not do.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Start, then hold the PM5 to the reader; press Stop to end.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ExportDialog</name>
    <message>
        <source>Export</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Export log</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Format</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>File name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Folder</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>→ %1/%2.%3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Pick a folder</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Glossary</name>
    <message>
        <source>Reference</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Anticollision: NFCID1 and SAK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>What a card reveals before any application is selected — the layer the Proxmark reaches on every card.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>SAK is the fastest tell: bit 0x20 means ISO 14443-4 and an ATS follows, 0x08/0x18 mean MIFARE Classic, 0x00 a Type 2 tag.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The anticollision identifier, 4/7/10 bytes (single/double/triple). A 4-byte value starting with 08 is not an identity — it is randomly generated per session, which payment cards, passports and identity cards all do on purpose.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The answer to the request command. Its top two bits encode the UID size; the low bits are the bit-frame anticollision setting.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>One byte, the answer to being singled out. 0x20 = speaks ISO 14443-4, ATS follows; 0x40 = NFC-DEP; 0x04 = UID not complete, another cascade level follows.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Type 2 tag — MIFARE Ultralight, NTAG213/215/216.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>MIFARE Classic 1K / Mini / 4K — Crypto1, reachable with the Proxmark (see the MIFARE page).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Plain ISO 14443-4: DESFire, bank cards, identity documents.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A Classic part plus an ISO 14443-4 part at once.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reading the ATS</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The Answer To Select, with which an ISO 14443-4 card introduces itself after activation. It says how to talk to the card, not what is on it.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>FSCI and FWI matter most: a small frame with a long waiting time is what makes probing slow.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Format byte. Low nibble = FSCI, the largest frame the card accepts (0→16 … 8→256 byte). Upper bits flag which of TA/TB/TC follow.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Transmission rates: which multiples of 106 kbit/s (212/424/848) the card supports, per direction.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Timing. Upper nibble FWI sets how long the card may take to answer (FWT ≈ 302 µs × 2^FWI); lower nibble SFGI the grace period after activation.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Protocol options: whether the card supports CID (card identifier) and NAD (node addressing).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>EMV / payment applications</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A contactless bank card is an ISO 14443-4 smart card. The reader selects applications by identifier and reads their public directory.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Nothing secret is readable: the keys never leave the card&apos;s secure element, and every real transaction needs a fresh cryptogram over an unpredictable terminal number.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>2PAY.SYS.DDF01 — the Proximity Payment System Environment. A directory listing the applications the card offers. German girocards often omit it, so known AIDs are tried directly.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Application Identifier, e.g. A0000000041010 = Mastercard, A0000000031010 = Visa, D276… = girocard.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A human-readable name the card gives its application, e.g. &quot;MASTERCARD&quot; or &quot;Debit&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>File Control Information — the response to selecting an application, a BER-TLV structure holding the label, priorities and processing options.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Status words</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Every APDU answer ends in two bytes, SW1 SW2. A card that refuses is not a card that is absent.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>6A82 means the application is genuinely not there. 6982 means it is there and will not talk — what a German eID says to any terminal without an authorisation certificate.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Success.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>File or application not found. Treated as absent.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Security status not satisfied — present, but authentication is missing.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Conditions of use not satisfied — right command, wrong moment.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Verification failed; a PIN retry counter just went down.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Success, xx more bytes waiting — fetch with GET RESPONSE.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Wrong Le; repeat with xx as the expected length.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>HfInfoPage</name>
    <message>
        <source>What the HF reader does</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Activates any ISO 14443-A card: UID/NFCID1, ATQA and SAK, and the ATS of a 14443-4 card</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Names the payment applications (PPSE, then known AIDs) and their labels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>On a payment card, reads the on-card transaction log when the card releases it</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Tells MIFARE Classic apart from a smart card by its SAK — the MIFARE page then attacks it</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Nothing secret. Keys stay in the card; a payment card cannot be cloned or replayed from a read.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A UID starting with 08 is random and regenerated each session — it is not the card and buys nothing at a terminal.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LfInfoPage</name>
    <message>
        <source>What the LF reader does</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reads EM4100, HID Prox and IO Prox 125 kHz badges</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Clones an EM4100 id onto a blank T5577 — a real duplicate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Encrypted LF (e.g. Hitag with keys) is not cloned; that is the Hitag page (planned).</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LfReference</name>
    <message>
        <source>Reference</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>125 kHz formats</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Low-frequency badges carry a fixed number and no cryptography — which is why they clone onto a blank so easily.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>40-bit read-only id, modulated RF/64. The classic building/fob transponder. Clones onto a T5577.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>HID 125 kHz Wiegand: a facility code plus a card number. Read-only in the tag.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Kantech ioProx, same idea as HID Prox with a different frame.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A programmable 125 kHz transponder that can emulate EM4100, HID and others. This is the blank the reader writes a cloned id onto.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The bit rate: one data bit per 64 carrier cycles. EM4100&apos;s standard clock.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LogPage</name>
    <message>
        <source>Export log</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Saved: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Export failed</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LogView</name>
    <message>
        <source>Log</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No activity yet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Export log</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Saved: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Export failed</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MainPage</name>
    <message>
        <source>Vicinity · SLIX</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Picopass</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Typ B</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>NFC-F</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Prime</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>LF · Hitag2/S</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>LF · read/write</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>LF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>860–960 MHz · Roadmap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>HF/LF Trace</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MifaInfoPage</name>
    <message>
        <source>What the MIFARE tools do</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check every sector against a dictionary of default keys</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Dump all blocks with the found keys, rebuilding the trailers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A card with custom keys will not open — that needs nested/hardnested (planned).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Writing/cloning to a magic card is planned.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MifareReference</name>
    <message>
        <source>Reference</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sectors, blocks, keys</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>MIFARE Classic 1K is 16 sectors of 4 blocks (16 byte each). Each sector is locked by its own two keys.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A sector opens only with the right key; the check tries a dictionary of well-known defaults.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>1K = 16 sectors × 4 blocks. Block 0 holds the UID; the last block of each sector is its trailer.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The 4th block: Key A (6 byte) · access bits (4) · Key B (6). Key A never reads back — the dump rebuilds it from the found key.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Two keys per sector. Access bits decide what each may do — often A reads, B reads and writes.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>MIFARE&apos;s proprietary 48-bit cipher. Broken since 2008; the phone&apos;s own NFC stack still refuses these cards, the Proxmark does not.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Factory/common keys like FFFFFFFFFFFF or A0A1A2A3A4A5. Many cheap cards are never re-keyed and open straight away.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>If default keys fail</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A card with custom keys will not open from the dictionary.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Statistical attacks that recover an unknown key from the card&apos;s own weak randomness. The firmware collects the nonces; the key search is heavy host work (planned).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A special blank whose block 0 / UID is writable — the target a dump is cloned onto (planned).</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PinDialog</name>
    <message>
        <source>Try PIN</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Offline PIN</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A wrong PIN spends one of the card&apos;s few tries. The app sends it only while at least 2 tries remain, so it can never block the card — but repeated wrong entries count the card down. Nothing is shown, logged or kept.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PIN (half-blind)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PIN</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Proxmark</name>
    <message>
        <source>Not connected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>open(%1) failed: %2 (errno %3)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>tcgetattr failed: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>tcsetattr failed: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>write failed: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No %1 - is the Proxmark plugged in?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Port open, probing...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Port opened but the Proxmark did not answer a PING. It may run older firmware that only speaks the legacy frame format.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Proxmark answered. Reading version...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Disconnected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Connected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Connected (no version reply)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Checking sector %1/%2...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sector %1  A:%2  B:%3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 of %2 sectors open with a default key</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Run the key check first</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Dumping sector %1/%2...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Dumped %1 of %2 blocks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reading 13.56 MHz - hold the card on the HF antenna...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Could not start HF read</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No card in the HF field</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No 13.56 MHz card found</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ATQA %1%2 · SAK %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 · UID %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Not a valid hex id: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Writing %1 to T5577 - hold the tag on the LF antenna...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Could not start the write</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Written: T5577 now answers as EM4100 %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Write rejected by the device (no tag, or not a T5577?)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No reply to the write - was a tag on the antenna?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Searching 125 kHz - hold the tag on the LF antenna...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Trying %1...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No 125 kHz badge found (blank tag, or an unsupported format)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PM5: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>EMV application</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Payment card (EMV)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>UID / NFCID1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>UID length</source>
        <translation type="unfinished"></translation>
    </message>
    <message numerus="yes">
        <source>%n byte(s)</source>
        <translation type="unfinished">
            <numerusform></numerusform>
            <numerusform></numerusform>
        </translation>
    </message>
    <message>
        <source>ATQA</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>SAK / SEL_RES</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>  · Crypto1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ATS</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ATS decoded</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ISO 14443-4 - probing applications...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ISO 14443-B</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PUPI / NFCID0</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ATQB</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Application data</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Protocol info</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Type B · PUPI %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ISO 14443-B card · PUPI %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No Type A card - trying Type B...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Card type</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>German eID, passport, many girocard</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>selectable</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>present · PACE/PIN required</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Card class</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>eID / passport / girocard (content needs PACE/keys)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Transaction %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Transaction log</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ROM · %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Firmware · %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Device · Proxmark 5 @ %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>present, protected — tap to try an offline PIN</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PIN</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>wrong PIN · %1 tries left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PIN blocked / not usable</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>verified ✓</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Trying offline PIN...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PIN attempt done</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>not tried — card hides its PIN-try counter, not risking a try</source>
        <translation type="unfinished"></translation>
    </message>
    <message numerus="yes">
        <source>not tried — only %n try(ies) left, would risk blocking</source>
        <translation type="unfinished">
            <numerusform></numerusform>
            <numerusform></numerusform>
        </translation>
    </message>
    <message>
        <source>card refuses offline PIN here (SW %1) — no try spent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>UID must be 4, 7 or 10 bytes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Could not start emulation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Emulating %1 — hold the PM5 to the reader, then Stop</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Emulation stopped</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>MIFARE Ultralight / NTAG (Type 2)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>MIFARE Classic 1K</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>MIFARE Classic Mini</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>MIFARE Classic 4K</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Infineon MIFARE Classic 1K</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>MIFARE Classic + ISO-DEP</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>MIFARE Plus 2K (SL2)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>MIFARE Plus 4K (SL2)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ISO 14443-4 (DESFire, Plus SL3, JCOP…)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>unknown (SAK %1)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TerminalPage</name>
    <message>
        <source>Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hold session</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Keep one activation across APDUs (SELECT → GPO → …)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>APDU (hex)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>cmd-id [payload] (hex)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Send</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Output</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Quick command</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Export log</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Saved: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Export failed</source>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
