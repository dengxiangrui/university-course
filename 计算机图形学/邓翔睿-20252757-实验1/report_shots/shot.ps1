Add-Type @"
using System;
using System.Text;
using System.Runtime.InteropServices;
public class Win32 {
    public delegate bool EnumProc(IntPtr h, IntPtr p);
    [DllImport("user32.dll")] public static extern bool EnumWindows(EnumProc cb, IntPtr p);
    [DllImport("user32.dll")] public static extern int GetWindowText(IntPtr h, StringBuilder s, int n);
    [DllImport("user32.dll")] public static extern bool IsWindowVisible(IntPtr h);
    [DllImport("user32.dll")] public static extern bool SetForegroundWindow(IntPtr h);
    [DllImport("user32.dll")] public static extern bool ShowWindow(IntPtr h, int cmd);
    [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr h, out RECT r);
    [DllImport("user32.dll")] public static extern bool SetCursorPos(int x, int y);
    [DllImport("user32.dll")] public static extern void keybd_event(byte vk, byte sc, uint f, IntPtr e);
    [DllImport("user32.dll")] public static extern void mouse_event(uint f, uint dx, uint dy, uint d, IntPtr e);
    [StructLayout(LayoutKind.Sequential)] public struct RECT { public int L, T, R, B; }
    public static IntPtr Find(string key) {
        IntPtr found = IntPtr.Zero;
        EnumWindows((h, p) => {
            var sb = new StringBuilder(512);
            GetWindowText(h, sb, 512);
            if (IsWindowVisible(h) && sb.ToString().Contains(key)) { found = h; return false; }
            return true;
        }, IntPtr.Zero);
        return found;
    }
    public static void PressKey(char c) {
        byte vk = (byte)((int)c & ~0x20);
        keybd_event(vk, 0, 0, IntPtr.Zero);
        keybd_event(vk, 0, 2, IntPtr.Zero);
    }
    public static void Drag(int x1, int y1, int x2, int y2) {
        SetCursorPos(x1, y1);
        mouse_event(0x0002, 0, 0, 0, IntPtr.Zero);
        for (int i = 1; i <= 12; i++) {
            SetCursorPos(x1 + (x2 - x1) * i / 12, y1 + (y2 - y1) * i / 12);
            System.Threading.Thread.Sleep(30);
        }
        mouse_event(0x0004, 0, 0, 0, IntPtr.Zero);
    }
}
"@
Add-Type -AssemblyName System.Drawing

$name = $args[0]
$keys = if ($args.Count -ge 2) { $args[1] } else { "" }
$drag = if ($args.Count -ge 3) { $args[2] } else { "" }

$h = [Win32]::Find("CG Lab1")
if ($h -eq [IntPtr]::Zero) { Write-Output "window not found"; exit 1 }
[Win32]::ShowWindow($h, 9) | Out-Null
[Win32]::SetForegroundWindow($h) | Out-Null
Start-Sleep -Milliseconds 700

if ($keys -ne "") { foreach ($c in $keys.ToCharArray()) { [Win32]::PressKey($c); Start-Sleep -Milliseconds 400 } }

if ($drag -ne "") {
    $r0 = New-Object Win32+RECT
    [Win32]::GetWindowRect($h, [ref]$r0) | Out-Null
    $ww = $r0.R - $r0.L; $hh = $r0.B - $r0.T
    $p = $drag.Split(',')
    $x1 = $r0.L + [int]($ww * [double]$p[0]); $y1 = $r0.T + [int]($hh * [double]$p[1])
    $x2 = $r0.L + [int]($ww * [double]$p[2]); $y2 = $r0.T + [int]($hh * [double]$p[3])
    [Win32]::Drag($x1, $y1, $x2, $y2)
    Start-Sleep -Milliseconds 500
}

$r = New-Object Win32+RECT
[Win32]::GetWindowRect($h, [ref]$r) | Out-Null
$w = $r.R - $r.L; $ht = $r.B - $r.T
$bmp = New-Object System.Drawing.Bitmap($w, $ht)
$g = [System.Drawing.Graphics]::FromImage($bmp)
$g.CopyFromScreen($r.L, $r.T, 0, 0, (New-Object System.Drawing.Size($w, $ht)))
$out = Join-Path $PSScriptRoot $name
$bmp.Save($out, [System.Drawing.Imaging.ImageFormat]::Png)
$g.Dispose(); $bmp.Dispose()
Write-Output "saved $name ($w x $ht)"
