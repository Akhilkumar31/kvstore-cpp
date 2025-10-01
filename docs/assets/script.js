document.addEventListener('DOMContentLoaded',()=>{
  document.querySelectorAll('pre').forEach(pre=>{
    const btn=document.createElement('button');
    btn.textContent='Copy';
    btn.className='copy-btn';
    Object.assign(btn.style,{position:'absolute',right:'10px',top:'8px',background:'#152444',color:'#cfe0ff',border:'1px solid #1a2947',borderRadius:'8px',padding:'4px 8px',cursor:'pointer'});
    const wrap=document.createElement('div');wrap.style.position='relative';
    pre.parentNode.insertBefore(wrap,pre);wrap.appendChild(pre);wrap.appendChild(btn);
    btn.addEventListener('click',()=>{navigator.clipboard.writeText(pre.innerText);btn.textContent='Copied!';setTimeout(()=>btn.textContent='Copy',1200);});
  });
});