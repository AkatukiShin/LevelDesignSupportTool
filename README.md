# LevelDesignSupportTool

## 概要
UE5向けのレベルデザイン支援プラグインです。  

複数アクターに対して  
- TransformOffset（固定値）
- RandomOffset（ランダム値）  

を組み合わせて一括適用でき、  
Previewによる事前確認とUndo/Redoにも対応している。

## Supported Version
- Unreal Engine 5.7
- Editor Only
- Windows 動作確認済み

## Installation
1. このリポジトリをダウンロードします
2. `LevelDesignSupportTool` フォルダをプロジェクトの `Plugins` フォルダに配置します  
   （なければ作成してください）
3. Unreal Editor を起動します
4. Plugins ウィンドウから「LevelDesignSupportTool」を有効化します
5. Editor を再起動します

## Open Tool
ツールは以下の方法で起動できます：
- 上記のInstalationの手順を行う
- すると、下画像で赤く囲まれたボタンが出現する。
![UE5Display](https://github.com/user-attachments/assets/2300b038-a764-46a4-be8c-50ee5331f93c)
- ボタンを押すと次のようなWindowが立ち上がる。  
<img width="505" height="464" alt="スクリーンショット 2026-04-13 110659" src="https://github.com/user-attachments/assets/5eeee7e7-9729-49da-8549-4c7d56030071" >

## UI Overview

ウィンドウは以下の構成になっています：

- Selected Actors  
  - 選択中アクターとクラスの一覧

- Preview  
  - 現在値 / 加算値 / 加算後の値を表示

- Transform Offset  
  - 固定値による位置・回転・スケールの加算

- Random Offset  
  - 範囲指定によるランダム加算

- Actions  
  - Preview / Apply / Reset Inputs

## How to Use

1. レベル上でアクターを複数選択します
2. ツールを起動します
3. Selected Actors に対象が表示されます
4. TransformOffset または RandomOffset を設定します（CheckBoxで適用対象を選択できます）
5. 「Preview」を押して結果を確認します
6. 「Apply」を押して変更を適用します(Previewを押さない場合も変更を適応可能ですが、結果確認ができません)
7. 「Reset Inputs」で入力値を初期化できます

## Input Behavior

### CheckBox
各オフセット設定には CheckBox があり、チェックが入っている項目のみ変更が適用されます。

例：
- Location のみチェック → 位置のみ変更
- Rotation / Scale をオフ → 回転・スケールは変更されない

---

### Step
Step 値は、+ / - ボタンで増減する際の変化量を決定します。

例：
- LocationStep = 10 → ボタン1回で10ずつ移動
- RotationStep = 45 → 45度ずつ回転

## TransformOffset
入力された値を選択中のアクターに対して加算する。  
ボタンによる値の増減にも対応しており、Stepの値を変更することでボタンを押した際に増減する値を設定することが可能。

## RandomOffset
入力された最小値から最大値の範囲でランダムな値を加算する。
各項目の左が最小値で右が最大値となっている。
Previewボタンを押すことで値を再抽選することが可能。
