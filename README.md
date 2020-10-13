Registration
===
The licensing DLL to be embedded with the target application

- Has the public key to verify the signature
- Installs last time key in registry
- Verifies file integrity
- Verifies license key

Requisites
---
- Checksums: ChekckParams.h
- Public key: PubKey_Blob.h
- Obscure key: Params.h
	
Interface
---

- GetLic
- GetHDInfo	
- TestPeriod
- TestHash
