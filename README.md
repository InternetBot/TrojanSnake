# Snake Game - Offensive Security Simulation Lab

This project is a research and educational simulation that uses a simple Snake game frontend to demonstrate how real world malware campaigns disguise themselves as legitimate software.

The purpose of this repository is to help security engineers, reverse engineers, and detection teams understand how trojanized applications operate, so that better antivirus, EDR, and behavioral detections can be built.

This is NOT a real malware distribution tool.  
It is a controlled laboratory environment designed for learning, research, and defensive development.

---

## CRITICAL WARNING

This repository is dangerous if misused.

It contains code that simulates attacker techniques such as:
- Payload staging  
- Remote code retrieval  
- In memory execution  
- Shellcode loading  
- Living off the land techniques  

These techniques are commonly used by real malware, red team tools, and advanced persistent threats.

You must only run this project in:
- A disposable virtual machine  
- A malware analysis sandbox  
- An isolated test environment  
- A research lab  

Never run this on:
- Your main computer  
- A work machine  
- A school network  
- A cloud server  
- Any system you do not own  

---

## Project Goal

The goal of this project is to study how attackers:
- Hide malicious logic inside legitimate software  
- Stage payloads from remote sources  
- Execute code without touching disk  
- Evade traditional antivirus scanning  

By recreating these techniques in a safe and controlled environment, defenders can:
- Build better detections  
- Improve behavioral monitoring  
- Write stronger YARA and EDR rules  
- Understand real attacker tradecraft  

---

## Technical Notes

This project models modern in memory payload delivery techniques commonly used by advanced malware loaders and red team tooling.

The research implementation is based on Donut style shellcode generation, which is a well known open source framework used to convert executables and DLLs into position independent shellcode that can be executed directly from memory.

This allows researchers to study:
- Fileless execution  
- In memory loaders  
- EDR and antivirus evasion techniques  
- Payload staging and execution chains  

To prevent misuse, critical implementation details, payload builders, and execution routines have been intentionally removed or disabled in this public repository.

The goal is to demonstrate how these techniques work without providing a copy paste weapon that could be used to harm real systems.

---

## How It Works

The Snake game acts as a benign decoy application.

Behind the scenes, the lab simulates:
- A staged payload download  
- Transformation into an in memory format  
- Execution inside the current process  

This models how modern malware loaders and droppers work without being deployed against real victims.

No real exploitation, lateral movement, or persistence is included.

---

## Prohibited Use

You may NOT:
- Use this on real users  
- Deploy this to the public  
- Package this as a real game  
- Upload this to marketplaces  
- Use this to access systems without permission  

This is strictly for:
- Security research  
- Malware analysis  
- Red team training  
- Blue team detection engineering  
- Academic study  

---

## Legal and Ethical Notice

This project is provided for educational and research purposes only.

By using this repository, you agree that:
- You are responsible for how you use it  
- You will only run it in authorized environments  
- You will not target real people or systems  

The author is not responsible for:
- Damage  
- Data loss  
- Account bans  
- Legal consequences  
- Policy violations  

Misuse of this code may violate:
- Computer Fraud and Abuse laws  
- University IT policies  
- Corporate security policies  
- GitHub Terms of Service  

Use responsibly.

---

## Why This Matters

Modern malware does not look like viruses anymore.  
It looks like:
- Games  
- Installers  
- Cheats  
- Tools  
- Launchers  
- Mods  

Understanding this is how we stop it.


---

## POC
## Screenshots

![Snake Game](Screenshot%202025-12-27%20105806.png)

![Snake Game](Screenshot%202025-12-27%20110040.png)

![Snake Game](Screenshot%202025-12-27%20105935.png)

![Snake Game](Screenshot%202025-12-27%20110208.png)
