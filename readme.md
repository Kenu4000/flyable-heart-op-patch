# Flyable Heart OP Patch (Source)

## Overview
This repository contains the source code for an unofficial patch that replaces the OP movie playback in *Flyable Heart* with an external player (mpv).

This repository does not include executable files.  
Prebuilt binaries can be downloaded from the following page:

Download:  
https://koma-buchi.com/wp-content/uploads/2026/03/FH_OP_patch.zip

---

## Important Notes

- This project is unofficial.
- All rights related to the original game belong to UNiSONSHIFT.
- The author of this project is not affiliated with UNiSONSHIFT.
- Please do not contact UNiSONSHIFT regarding this project.
- Use at your own risk.

---

## Included Files

- `src/pal_patch.cpp`
- `src/patch.cpp`
- `src/patch.rc`
- `src/gen_exports.py`
- `src/mpv_input.conf`

---

## Build Overview

1. Run `gen_exports.py` to generate `exports.inc`
2. Build `pal_patch.cpp` to generate the proxy `PAL.dll`
3. Build `patch.rc` and `patch.cpp` to generate `patch.exe`

---

## Distribution Policy

Redistribution of prebuilt binaries is prohibited.  
Please obtain releases only from the official distribution page linked above.

---

## License

The license for the source code contained in this repository can be found in the `LICENSE` file.

# Flyable Heart OP Patch (Source)

## 概要
Flyable HeartのOP再生を外部プレイヤー（mpv）に置き換える非公式パッチのソースコードです。

このリポジトリには実行ファイルは含まれていません。  
配布版は以下の配布ページから取得してください。

配布ページ:
https://koma-buchi.com/wp-content/uploads/2026/03/FH_OP_patch.zip

## 注意事項
- 本プロジェクトは非公式のものです
- ゲーム本体の著作権はユニゾンシフト様に帰属します
- 作者はユニゾンシフト様とは一切関係ありません
- 本プロジェクトに関する問い合わせをユニゾンシフト様へ行わないでください
- 使用は自己責任でお願いします

## 収録内容
- `src/pal_patch.cpp`
- `src/patch.cpp`
- `src/patch.rc`
- `src/gen_exports.py`
- `src/mpv_input.conf`

## ビルド概要
1. `gen_exports.py` を実行して `exports.inc` を生成
2. `pal_patch.cpp` をビルドして proxy `PAL.dll` を生成
3. `patch.rc` と `patch.cpp` から `patch.exe` を生成

## 配布について
配布版バイナリの再配布は禁止します。  
入手は配布ページからお願いします。

## ライセンス
このリポジトリに含まれるコードのライセンスは `LICENSE` を参照してください。
